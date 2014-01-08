function CrateTick(_actor1)

	-- Get the scene
	local scene = scene.find("Scene1");
	
	-- Check icons I1 to I10 for collision with the tester icon actor
	for i=1,10 do
	
		-- Get the icon actor
		local name = "I" .. i;
		local actor2 = actor.find(name, scene);
		
		-- Test to see if tester and the icon actor overlaps
		if (actor.overlaps(_actor1, actor2, false)) then
			actor.set(actor2, "BackgroundColour", vec.new(255, 255, 80, 255));
		else
			actor.set(actor2, "BackgroundColour", vec.new(255, 255, 255, 255));
		end
	
	end

	return 1;
	
end