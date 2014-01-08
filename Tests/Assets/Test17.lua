local total_objects = 0
local last_time = 0
local crate_template = nil

function Scene_OnTick(_object)

	local touch_info = input.touchInfo(0)
	if (touch_info.z ~= 0) then
		local scene_pos = scene.toScene(_object, touch_info.x, touch_info.y, true)
		print(scene_pos.x .. ", " .. scene_pos.y)
	end

	-- Dont allow more than 100 objects to be spawned
	if (total_objects > 100) then 
		return 1
	end
	
	local this_time =  os.time()

	-- Spawn a new object every few seconds
	if ((this_time - last_time) > 1) then
	
--		print("Spawning new crate")

		-- Find template
		if (crate_template == nil) then
			crate_template = template.find("CrateTemplate", _object)
			print("Cached tenmplate")
		end
		
		-- Create template parameters
		local params = {}
		params["pos"] = "0, -400"
		
		-- Instantiate the template
		template.from(crate_template, _object, params)
		
		total_objects = total_objects + 1
		
		last_time = this_time
		
	
	end

	return 1
	
end

function CollisionStarted(_actor)

	local collisions_started = actor.getStartContacts(_actor)
	if (collisions_started == nil) then
		return 1
	end

	for key,collider in ipairs(collisions_started) do
		if (actor.get(collider, "Type") == 1) then
			actor.set(collider, "Colour", "255, 0, 0, 255")
		end
	end	
	
--	actor.set(_actor, "Force", "0, 1")

	return 1
end

function CollisionEnded(_actor)

	local collisions_ended = actor.getEndContacts(_actor)
	if (collisions_ended == nil) then
		return 1
	end
	
	for key,collider in ipairs(collisions_ended) do
		if (actor.get(collider, "Type") == 1) then
			actor.set(collider, "Colour", "255, 255, 255, 255")
		end
	end	

	return 1
end


