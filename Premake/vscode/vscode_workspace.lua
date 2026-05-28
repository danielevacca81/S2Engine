--
-- Name:        vscode_workspace.lua
-- Purpose:     Generate a vscode file.
-- Author:      Ryan Pusztai
-- Modified by: Andrea Zanellato
--              Manu Evans
--              Yehonatan Ballas
-- Created:     2013/05/06
-- Copyright:   (c) 2008-2020 Jason Perkins and the Premake project
--

local p = premake
local project = p.project
local workspace = p.workspace
local tree = p.tree
local vscode = p.modules.vscode

vscode.workspace = {}
local m = vscode.workspace

function m.getcompiler(cfg)
	local toolset = p.tools[_OPTIONS.cc or cfg.toolset or p.CLANG]
	if not toolset then
		error("Invalid toolset '" + (_OPTIONS.cc or cfg.toolset) + "'")
	end
	return toolset
end

--
-- Generate a vscode file
--
function m.generate(wks)
	p.utf8()
	p.w('{"folders": [')

	-- workspace should be first for clangd to use it as the working directory
	p.w('{')
	p.w('"path": "."')
	p.w('},')

	--
	-- Project list
	--
	local root_src_dirs = {}
	local tr = workspace.grouptree(wks)
	tree.traverse(tr, {
		onleaf = function(n)
			local prj = n.project

			if prj.workspace.location ~= prj.location then
				local prjpath = path.getrelative(prj.workspace.location, prj.location)
				p.w('{')
				p.w('"path": "%s"', prjpath)
				p.w('},')
			end

			-- add root source file directories
			local non_root_path = '' -- used to remove the non root part from the end of a leaf node's path
			local tr = project.getsourcetree(prj)
			tree.traverse(tr, {
				onbranchenter = function(node, depth)
					if depth ~= 0 then
						non_root_path = non_root_path .. '/' .. node.name
					end
				end,
				onbranchexit = function(node, depth)
					if depth ~= 0 then
						non_root_path = non_root_path:sub(1, non_root_path:len()-(node.name:len()+1))
					end
				end,
				onleaf = function(node, depth)
					if node.relpath == nil then
						return
					end
					non_root_path = non_root_path ..'/'.. node.name
					local rel_root_path = node.relpath:sub(1, node.relpath:len()-(non_root_path:len()))
					non_root_path = non_root_path:sub(1, non_root_path:len()-(node.name:len()+1))
					root_src_dirs[rel_root_path] = true
				end
			})
		end,
	})

	for src_dir_rel in pairs(root_src_dirs) do
		print(src_dir_rel)
		p.w('{')
		p.w('"path": "%s"', src_dir_rel)
		p.w('},')
	end

	-- for clangd to find compile_commands.json in the build dir
	p.w('],')
	p.w('"settings":{')
	p.w('"clangd.arguments":[')
	p.w('"--compile-commands-dir=.",')
	p.w('"--background-index",')
	p.w('"--header-insertion=never"')
	p.w(']}')

	p.w('}')

	--TODO wks.startproject
end

function m.generate_tasks(wks)
    p.utf8()
    _p('{')
    _p(1, '"version": "2.0.0",')
    _p(1, '"tasks": [')

    local first_task = true
    local tr = workspace.grouptree(wks)
    tree.traverse(tr, {
        onleaf = function(n)
            local prj = n.project
            for cfg in project.eachconfig(prj) do
                if not first_task then
                    _p(1, ',{')
                else
                    _p(1, '{')
                    first_task = false
                end

                if cfg.system == p.WINDOWS then
                    local msbuild_cmd = "$msbuild = & 'C:\\\\Program Files (x86)\\\\Microsoft Visual Studio\\\\Installer\\\\vswhere.exe' -latest -requires Microsoft.Component.MSBuild -find 'MSBuild\\\\**\\\\Bin\\\\MSBuild.exe' | Select-Object -First 1; & $msbuild '${workspaceFolder}\\\\" .. wks.name .. ".sln' /p:Configuration=" .. cfg.buildcfg .. " /p:Platform=x64 /m /nologo"
                    _p(2, '"label": "Build %s (%s)",', prj.name, cfg.buildcfg)
                    _p(2, '"type": "process",')
                    _p(2, '"command": "powershell.exe",')
                    _p(2, '"args": [')
                        _p(3, '"-NoProfile",')
                        _p(3, '"-ExecutionPolicy", "Bypass",')
                        _p(3, '"-Command",')
                        _p(3, '"%s"', msbuild_cmd)
                    _p(2, '],')
                    _p(2, '"group": "build",')
                    _p(2, '"problemMatcher": "$msCompile",')
                    _p(2, '"presentation": { "reveal": "always", "panel": "shared" }')
                else
                    _p(2, '"label": "Build %s (%s)",', prj.name, cfg.buildcfg)
                    _p(2, '"type": "shell",')
                    if os.isfile(prj.location .. '/build.ninja') then
                        _p(2, '"command": "clear && time ninja",')
                    else
                        _p(2, '"command": "clear && time make %s config=%s -r -j$(nproc)",', prj.name, cfg.buildcfg:lower())
                    end
                    _p(2, '"args": [],')
                    _p(2, '"group": "build",')
                    _p(2, '"problemMatcher": "$gcc",')
                    _p(2, '"presentation": { "reveal": "always", "panel": "shared" }')
                end
                _p(1, '}')
            end

            -- Clean task (Windows)
            if prj.system == p.WINDOWS or os.target() == "windows" then
                _p(1, ',{')
                local clean_cmd = "$msbuild = & 'C:\\\\Program Files (x86)\\\\Microsoft Visual Studio\\\\Installer\\\\vswhere.exe' -latest -requires Microsoft.Component.MSBuild -find 'MSBuild\\\\**\\\\Bin\\\\MSBuild.exe' | Select-Object -First 1; & $msbuild '${workspaceFolder}\\\\" .. wks.name .. ".sln' /t:Clean /p:Platform=x64 /m /nologo"
                _p(2, '"label": "Clean %s",', prj.name)
                _p(2, '"type": "process",')
                _p(2, '"command": "powershell.exe",')
                _p(2, '"args": [')
                    _p(3, '"-NoProfile",')
                    _p(3, '"-ExecutionPolicy", "Bypass",')
                    _p(3, '"-Command",')
                    _p(3, '"%s"', clean_cmd)
                _p(2, '],')
                _p(2, '"problemMatcher": "$msCompile",')
                _p(2, '"presentation": { "reveal": "always", "panel": "shared" }')
                _p(1, '}')
            end
        end,
    })
    _p(1, ']')
    _p('}')
end

function m.generate_launch(wks)
	p.utf8()
	_p('{')
	_p(1, '"configurations": [')

	local first_cfg = true
	local tr = workspace.grouptree(wks)
	tree.traverse(tr, {
		onleaf = function(n)
			local prj = n.project
			for cfg in project.eachconfig(prj) do
				if first_cfg then
					first_cfg = false
					_p(1, '{')
				else
					_p(1, ',{')
				end

				if cfg.system == p.WINDOWS then
					local targetdir = path.getrelative(wks.location, cfg.buildtarget.directory)
					_p(2, '"name": "%s (%s)",', prj.name, cfg.buildcfg)
					_p(2, '"type": "cppvsdbg",')
					_p(2, '"request": "launch",')
					_p(2, '"program": "${workspaceFolder}/%s/%s.exe",', targetdir, prj.name)
					_p(2, '"args": [],')
					_p(2, '"cwd": "${workspaceFolder}",')
					_p(2, '"preLaunchTask": "Build %s (%s)"', prj.name, cfg.buildcfg)
				else
					local targetdir = path.getrelative(wks.location, cfg.buildtarget.directory)
					_p(2, '"name": "%s (%s)",', prj.name, cfg.buildcfg)
					_p(2, '"type": "cppdbg",')
					_p(2, '"request": "launch",')
					_p(2, '"program": "${workspaceFolder}/%s/%s",', targetdir, prj.name)
					_p(2, '"args": [],')
					_p(2, '"cwd": "${workspaceFolder}",')
					_p(2, '"MIMode": "gdb",')
					_p(2, '"preLaunchTask": "Build %s (%s)"', prj.name, cfg.buildcfg)
				--_p(2, '"externalConsole": false,')
				-- _p(2, '"MIMode": "gdb",')
				-- _p(2, '"setupCommands": [')
				-- 	_p(3, '{')
				-- 	_p(3, '"description": "Enable pretty-printing for gdb",')
				-- 	_p(3, '"text": "-enable-pretty-printing",')
				-- 	_p(3, '"ignoreFailures": true')
				-- 	_p(3, '},')
				-- 	_p(3, '{')
				-- 	_p(3, '"description": "Enable break on all-exceptions",')
				-- 	_p(3, '"text": "catch throw",')
				-- 	_p(3, '"ignoreFailures": true')
				-- 	_p(3, '}')
				-- _p(2, '],')
				-- _p(2, '"miDebuggerPath": "/usr/bin/gdb",')
				end
				_p(1, '}')
			end
		end,
	})

	_p(1, ']')
	_p('}')
end

function m.c_cpp_properties(wks)
	p.utf8()
	_p('{')
	_p(1, '"configurations": [')

	local first_cfg = true
	local tr = workspace.grouptree(wks)
	tree.traverse(tr, {
		onleaf = function(n)
			local prj = n.project
			for cfg in project.eachconfig(prj) do
				if first_cfg then
					first_cfg = false
					_p(1, '{')
				else
					_p(1, ',{')
				end
				_p(2, '"name": "%s (%s)",', prj.name, cfg.buildcfg)
				_p(2, '"includePath": [')
				_p(3, '"${workspaceFolder}/**"')
				for _, includedir in ipairs(cfg.includedirs) do
					local reldir = path.getrelative(wks.location, includedir)
					_p(3, ',"${workspaceFolder}/%s"', reldir)
				end
				_p(2, '],')
				_p(2, '"defines": [')
				if #cfg.defines > 0 then
					_p(3, '"%s"', cfg.defines[1]:gsub('"','\\"'))
					for i = 2,#cfg.defines do
						_p(3, ',"%s"', cfg.defines[i]:gsub('"','\\"'))
					end
				end
				_p(2, '],')
				if cfg.system == p.WINDOWS then
					_p(2, '"compilerPath": "cl.exe",')
					_p(2, '"intelliSenseMode": "windows-msvc-x64",')
				else
					_p(2, '"compilerPath": "/usr/bin/g++",')
					_p(2, '"intelliSenseMode": "linux-gcc-x64",')
				end
				if cfg.cdialect ~= nil then
					_p(2, '"cStandard": "%s",', cfg.cdialect:lower())
				end
				if cfg.cppdialect ~= nil then
					_p(2, '"cppStandard": "%s",', cfg.cppdialect:lower())
				end
				_p(2, '"configurationProvider": "ms-vscode.cmake-tools"')
				_p(1, '}')
			end
		end,
	})

	_p(1, '],')
	_p(1, '"version": 4')
	_p('}')
end
