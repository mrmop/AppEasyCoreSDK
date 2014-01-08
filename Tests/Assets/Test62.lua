compass_started = false;

function Scene_OnTick(_scene)

	-- If compass not started then start it
	if (compass_started == false) then
		input.startCompass();
		compass_started = true;
	end

	-- Get compass position
	local comp = input.getCompass();
	
	-- Get the XOML variables that are used to display the compass data
	local heading_x = variable.find("heading_x", _scene);
	local heading_y = variable.find("heading_y", _scene);
	local heading_z = variable.find("heading_z", _scene);
	local direction = variable.find("direction", _scene);
	
	-- Set the variables with the compass data
	variable.set(heading_x, comp.x);
	variable.set(heading_y, comp.y);
	variable.set(heading_z, comp.z);
	variable.set(direction, comp.w);

	return 1;
end