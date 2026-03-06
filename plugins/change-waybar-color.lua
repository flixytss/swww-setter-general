local home = os.getenv("HOME")
local img = arg[1]
if not home or not img then return end

local flag = img:match("%.png$") and "-png" or "-jpeg"
local handle = io.popen(string.format("main-color %s \"%s\"", flag, img))
local result = handle:read("*a")
handle:close()

local rgb = {}
for color in result:gmatch("%S+") do
    table.insert(rgb, color)
end

if #rgb >= 3 then
    local new_line = string.format("@define-color bg rgba(%s, %s, %s, 0.7);\n", rgb[1], rgb[2], rgb[3])
    local css_path = home .. "/.config/waybar/style.css"
    
    local lines = {}
    local f_r = io.open(css_path, "r")
    if f_r then
        for line in f_r:lines() do
            table.insert(lines, line .. "\n")
        end
        f_r:close()
    end

    if #lines > 0 then
        lines[1] = new_line
    else
        lines = {new_line}
    end

    local f_w = io.open(css_path, "w")
    if f_w then
        f_w:write(table.concat(lines))
        f_w:close()
    end

    os.execute("killall waybar && nohup waybar > /dev/null 2>&1 &")
end