function ChangeProperties(_actor)

	-- Get actor property list
	local prop_list = actor.get(_actor, "UserProperties")
	-- Get the properties
	local p1 = userprops.find(prop_list, "MyProperty1")
	local p2 = userprops.find(prop_list, "MyProperty2")
	local p3 = userprops.find(prop_list, "MyProperty3")
	-- Print the properties values
	print("p1=" .. userprops.get(p1))
	print("p2=" .. userprops.get(p2))
	print("p3=" .. userprops.get(p3))
	-- Add something onto the properties
	userprops.add(p1, 1)
	userprops.add(p2, 0.5)
	userprops.add(p3, "o")

	-- Get scene 1
	local scene1 = scene.find("Scene1")
	-- Get scene 1 propert list
	local scene_prop_list = scene.get(scene1, "UserProperties")
	-- Get counter property, display and increment
	local counter_prop = userprops.find(scene_prop_list, "Counter")
	print("Counter=" .. userprops.get(counter_prop))
	userprops.add(counter_prop, 1)

end