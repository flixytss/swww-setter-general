-- Obtener el directorio HOME
local home = os.getenv("HOME")
if not home then return end

local initback_path = home .. "/.local/state/initback.sh"
local args_path = home .. "/.local/state/swww-setter-args"

local filer = io.open(args_path, "r")
local swww_args = ""
if filer then
    swww_args = filer:read("*a"):gsub("[\n\r]", "")
    filer:close()
end

local file = io.open(initback_path, "w")
if file then
    local content = string.format("#!/bin/bash\nswww img \"%s\" %s", arg[1], swww_args)
    file:write(content)
    file:close()
end

os.execute(initback_path)