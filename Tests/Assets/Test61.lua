acceleromeer_started = false;

function Scene_OnTick(_scene)

	-- If accelerometer not started then start it
	if (acceleromeer_started == false) then
		input.startAccelerometer();
		acceleromeer_started = true;
	end

	-- Get accelerometer position
	local acc = input.getAccelerometer();
	
	-- Get the XOML variables that are used to display the accelerometer data
	local accel_x = variable.find("accel_x", _scene);
	local accel_y = variable.find("accel_y", _scene);
	local accel_z = variable.find("accel_z", _scene);
	
	-- Set the variabls with the accelerometer data
	variable.set(accel_x, acc.x);
	variable.set(accel_y, acc.y);
	variable.set(accel_z, acc.z);

	return 1;
end