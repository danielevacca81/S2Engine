--
-- Name:        vscode_workspace.lua
-- Purpose:     Generate a vscode file.
-- Author:      Ryan Pusztai
-- Modified by: Andrea Zanellato
--              Manu Evans
--              Yehonatan Ballas
--				Daniele Vacca
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
        error("Invalid toolset '" .. tostring(_OPTIONS.cc or cfg.toolset) .. "'")
    end
    return toolset
end

-- Generate a vscode file
function m.generate(wks)
    p.utf8()
    p.w('{"folders": [')

    -- workspace should be first for clangd to use it as the working directory
    p.w('{')
    p.w('"path": "."')
    p.w('},')

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

            local non_root_path = '' 
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

    p.w('],')
    p.w('"settings":{')
    p.w('"clangd.arguments":[')
    p.w('"--compile-commands-dir=.",')
    p.w('"--background-index",')
    p.w('"--header-insertion=never"')
    p.w(']}')
    p.w('}')
end

function m.generate_tasks(wks)
    p.utf8()
    _p('{')
    _p(1, '"version": "2.0.0",')
    _p(1, '"tasks": [')

    local first_task = true
    local tr = workspace.grouptree(wks)
    local build_dir = path.getrelative(wks.basedir, wks.location)
    
    local vscode_build_path = "${workspaceFolder}"
    if build_dir ~= "." then
        vscode_build_path = vscode_build_path .. "/" .. build_dir
    end

    tree.traverse(tr, {
        onleaf = function(n)
            local prj = n.project
            for cfg in project.eachconfig(prj) do
                if not first_task then _p(1, ',{') else _p(1, '{') first_task = false end

                if cfg.system == p.WINDOWS then
                    local sln_path = path.translate(vscode_build_path .. "/" .. wks.name .. ".sln", "\\")
                    
                    _p(2, '"label": "Build %s (%s)",', prj.name, cfg.buildcfg)
                    _p(2, '"type": "process",')
                    _p(2, '"command": "powershell.exe",')
                    _p(2, '"args": [')
                        _p(3, '"-NoProfile", "-ExecutionPolicy", "Bypass", "-Command",')
                        _p(3, '"& \'C:\\\\Program Files (x86)\\\\Microsoft Visual Studio\\\\2022\\\\BuildTools\\\\MSBuild\\\\Current\\\\Bin\\\\MSBuild.exe\' \'%s\' /p:Configuration=%s /p:Platform=x64 /m /nologo"', sln_path, cfg.buildcfg)
                    _p(2, '],')
                    _p(2, '"group": { "kind": "build", "isDefault": true },')
                    _p(2, '"problemMatcher": "$msCompile",')
                    _p(2, '"presentation": { "reveal": "always", "panel": "shared" }')
                else
                    _p(2, '"label": "Build %s (%s)",', prj.name, cfg.buildcfg)
                    _p(2, '"type": "shell",')
                    _p(2, '"command": "make",')
                    _p(2, '"args": ["%s", "config=%s"],', prj.name, cfg.buildcfg:lower())
                    _p(2, '"options": {')
                    _p(3, '"cwd": "%s"', vscode_build_path)
                    _p(2, '},')
                    _p(2, '"group": { "kind": "build", "isDefault": true },')
                    _p(2, '"problemMatcher": "$gcc",')
                    _p(2, '"presentation": { "reveal": "always", "panel": "shared" }')
                end
                _p(1, '}')
            end


            local sys = nil
            for cfg in project.eachconfig(prj) do
                sys = cfg.system
                break
            end
            
            _p(1, ',{')
            _p(2, '"label": "Clean %s",', prj.name)
            
            if sys == p.WINDOWS then
                local sln_path = path.translate(vscode_build_path .. "/" .. wks.name .. ".sln", "\\")
                _p(2, '"type": "process",')
                _p(2, '"command": "powershell.exe",')
                _p(2, '"args": [')
                    _p(3, '"-NoProfile", "-ExecutionPolicy", "Bypass", "-Command",')
                    _p(3, '"& \'C:\\\\Program Files (x86)\\\\Microsoft Visual Studio\\\\2022\\\\BuildTools\\\\MSBuild\\\\Current\\\\Bin\\\\MSBuild.exe\' \'%s\' /t:Clean /p:Platform=x64 /m /nologo"', sln_path)
                _p(2, '],')
                _p(2, '"problemMatcher": "$msCompile",')
            else
                _p(2, '"type": "shell",')
                _p(2, '"command": "make",')
                _p(2, '"args": ["clean"],')
                _p(2, '"options": { "cwd": "%s" },', vscode_build_path)
                _p(2, '"problemMatcher": "$gcc",')
            end
            
            _p(2, '"presentation": { "reveal": "always", "panel": "shared" }')
            _p(1, '}')
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
                if first_cfg then first_cfg = false _p(1, '{') else _p(1, ',{') end
                
                _p(2, '"name": "%s (%s)",', prj.name, cfg.buildcfg)
                _p(2, '"includePath": [')
                _p(3, '"${workspaceFolder}/**"')
                for _, includedir in ipairs(cfg.includedirs) do
                    local reldir = path.getrelative(wks.basedir, includedir)
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
                    _p(2, '"intelliSenseMode": "windows-msvc-x64"') 
                else
                    _p(2, '"compilerPath": "/usr/bin/gcc",')
                    _p(2, '"intelliSenseMode": "linux-gcc-x64"') 
                end
                
                if cfg.cdialect ~= nil then
                    _p(2, ',"cStandard": "%s"', cfg.cdialect:lower())
                end
                if cfg.cppdialect ~= nil then
                    _p(2, ',"cppStandard": "%s"', cfg.cppdialect:lower())
                end
                _p(1, '}')
            end
        end,
    })

    _p(1, '],')
    _p(1, '"version": 4')
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
                if first_cfg then first_cfg = false _p(1, '{') else _p(1, ',{') end

                local targetdir = path.getrelative(wks.basedir, cfg.buildtarget.directory)

                if cfg.system == p.WINDOWS then
                    _p(2, '"name": "%s (%s)",', prj.name, cfg.buildcfg)
                    _p(2, '"type": "cppvsdbg",')
                    _p(2, '"request": "launch",')
                    _p(2, '"program": "${workspaceFolder}/%s/%s.exe",', targetdir, prj.name)
                    _p(2, '"args": [],')
                    _p(2, '"cwd": "${workspaceFolder}/%s",', targetdir)
                    _p(2, '"preLaunchTask": "Build %s (%s)"', prj.name, cfg.buildcfg)
                else
                    _p(2, '"name": "%s (%s)",', prj.name, cfg.buildcfg)
                    _p(2, '"type": "cppdbg",')
                    _p(2, '"request": "launch",')
                    _p(2, '"program": "${workspaceFolder}/%s/%s",', targetdir, prj.name)
                    _p(2, '"args": [],')
                    _p(2, '"cwd": "${workspaceFolder}/%s",', targetdir)
                    _p(2, '"MIMode": "gdb",')
                    _p(2, '"preLaunchTask": "Build %s (%s)"', prj.name, cfg.buildcfg)
                end
                _p(1, '}')
            end
        end,
    })

    _p(1, ']')
    _p('}')
end