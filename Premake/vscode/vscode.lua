--
-- Name:        vscode.lua
-- Purpose:     Define the vscode action(s).
-- Author:      Ryan Pusztai
-- Modified by: Andrea Zanellato
--              Andrew Gough
--              Manu Evans
--              Jason Perkins
--              Yehonatan Ballas
-- Created:     2013/05/06
-- Copyright:   (c) 2008-2020 Jason Perkins and the Premake project
--

local p = premake

p.modules.vscode = {}
p.modules.vscode._VERSION = p._VERSION

local vscode = p.modules.vscode
local project = p.project


function vscode.generateWorkspace(wks)
    p.eol("\r\n")
    p.indent("  ")

    local vscode_dir = path.join(wks.basedir, ".vscode")

    --p.generate(wks, path.join(wks.basedir, wks.name .. ".code-workspace"), vscode.workspace.generate)
    p.generate(wks, path.join(vscode_dir, "tasks.json"), vscode.workspace.generate_tasks)
    p.generate(wks, path.join(vscode_dir, "launch.json"), vscode.workspace.generate_launch)
    p.generate(wks, path.join(vscode_dir, "c_cpp_properties.json"), vscode.workspace.c_cpp_properties)
end

function vscode.cleanWorkspace(wks)
    p.clean.file(wks, path.join(wks.basedir, wks.name .. ".code-workspace"))
end

function vscode.cleanProject(prj)
    p.clean.directory(path.join(prj.basedir, ".vscode"))
end

include("vscode_workspace.lua")

include("_preload.lua")

return vscode