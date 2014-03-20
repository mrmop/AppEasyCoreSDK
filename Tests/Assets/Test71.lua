local total_objects = 0
local last_time = 0
local crate_template = nil

local angle = 0
local alpha = 0

local vertices = {}
local uvs = {}
local indices = {}
local colours = {}

--[[local vertices = 
{
	vec.new(-100, -100, -100, 0),
	vec.new(100, -100, -100, 0),
	vec.new(100, 100, -100, 0),
	vec.new(-100, 100, -100, 0),
	
	vec.new(100, -100, 100, 0),
	vec.new(-100, -100, 100, 0),
	vec.new(-100, 100, 100, 0),
	vec.new(100, 100, 100, 0),
	
	vec.new(100, -100, -100, 0),
	vec.new(100, -100, 100, 0),
	vec.new(100, 100, 100, 0),
	vec.new(100, 100, -100, 0),

	vec.new(-100, -100, 100, 0),
	vec.new(-100, -100, -100, 0),
	vec.new(-100, 100, -100, 0),
	vec.new(-100, 100, 100, 0),
}

local uvs = 
{
	vec.new(0,0,0,0),
	vec.new(1,0,0,0),
	vec.new(1,1,0,0),
	vec.new(0,1,0,0),

	vec.new(0,0,0,0),
	vec.new(1,0,0,0),
	vec.new(1,1,0,0),
	vec.new(0,1,0,0),

	vec.new(0,0,0,0),
	vec.new(1,0,0,0),
	vec.new(1,1,0,0),
	vec.new(0,1,0,0),

	vec.new(0,0,0,0),
	vec.new(1,0,0,0),
	vec.new(1,1,0,0),
	vec.new(0,1,0,0),
}]]--

--local indices = {0, 1, 2, 3, 4, 5, 6, 7}


grid_width = 50
grid_height = 50
grid_cell_width = 100
grid_cell_height = 100
grid_generated = false
function GeneratGrid(_scene)
	if (grid_generated) then return end

	local crate = actor.find("crate1", _scene)
	
	local depths = {}
	for t=1,(grid_width + 1) * (grid_height + 1) do
		depths[t] = math.random(100)
	end
	
	local rgba = {}
	for t=1,(grid_width + 1) * (grid_height + 1) do
--		rgba[t] = vec.new(math.random(255), math.random(255), math.random(255), 255)
		local shade = ((100 - depths[t]) * 255 / 100)
		rgba[t] = vec.new(shade, shade, shade, 255)
	end
	
	-- Generate vertices
	local index = 1
	local zindex = 1
	local yp = -grid_height / 2 * grid_cell_height
	for y=1,grid_height do
		local xp = -grid_width / 2 * grid_cell_width
		for x=1,grid_width do
			local z1 = (y-1)*grid_width+(x-1)
			local z2 = y*grid_width+(x-1)
			
			vertices[index] = vec.new(xp, yp, depths[z1])
			vertices[index + 1] = vec.new(xp + grid_cell_width, yp, depths[z1 + 1])
			vertices[index + 2] = vec.new(xp + grid_cell_width, yp + grid_cell_height, depths[z2 + 1])
			vertices[index + 3] = vec.new(xp, yp + grid_cell_height, depths[z2])
			colours[index] = rgba[z1]
			colours[index + 1] = rgba[z1 + 1]
			colours[index + 2] = rgba[z2 + 1]
			colours[index + 3] = rgba[z2]
			index = index + 4
			xp = xp + grid_cell_width
		end
		yp = yp + grid_cell_height
	end
	actor.setRGBA(crate, 0, colours)
	
	-- Generate UVs
	index = 1
	for t=1,grid_height * grid_width do
		uvs[index] = vec.new(0,0,0,0)
		uvs[index + 1] = vec.new(1,0,0,0)
		uvs[index + 2] = vec.new(1,1,0,0)
		uvs[index + 3] = vec.new(0,1,0,0)
		index = index + 4
	end
	actor.setUV(crate, 0, uvs)
	
	grid_generated = true

end
local cam_pos_x = 0

function Scene_OnTick(_object)

	GeneratGrid(_object)

	local touch_info = input.touchInfo(0)
	if (touch_info.z ~= 0) then
		local scene_pos = scene.toScene(_object, touch_info.x, touch_info.y, true)
	end
	local cam_pos = scene.get(_object, "Position")
	
	local crate = actor.find("crate1", _object)
	local mat = matrix.create()
	local trans_mat = matrix.create()
	matrix.rotate(mat, -10, 0, angle)
	matrix.translate(trans_mat, -cam_pos.x + cam_pos_x,  -cam_pos.y, 150)
	matrix.postMultiply(mat, trans_mat)
	local projected_v = matrix.transformProject(mat, vertices, 300)
	print(matrix.getAt(mat, 0))
	actor.setVertex(crate, 0, projected_v)
	actor.setRGBA(crate, 0, 255, 255, 255, alpha)
	actor.setRGBA(crate, 1, alpha, 255, 255, 255)
	actor.setRGBA(crate, 2, 255, alpha, 255, 255)
	actor.setRGBA(crate, 3, 255, 255, alpha, 255)
	actor.setRGBA(crate, 4, 255, 255, 255, alpha)
	actor.setRGBA(crate, 5, alpha, 255, 255, 255)
	actor.setRGBA(crate, 6, 255, alpha, 255, 255)
	actor.setRGBA(crate, 7, 255, 255, alpha, 255)
	matrix.destroy(trans_mat)
	matrix.destroy(mat)
--	angle = angle + 1
	alpha = alpha + 1
	cam_pos_x = cam_pos_x + 1
	if (cam_pos_x > (grid_cell_width * grid_width) / 2) then cam_pos_x = -(grid_cell_width * grid_width) / 2 end
	if (alpha > 255) then alpha = 0 end

	return 1
end


