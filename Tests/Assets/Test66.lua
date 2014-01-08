function CreateXomlObjectsFromLua(_scene)
	-- Create a XOML label from a table
	local label = resource.create("Label", {
		Name="Label1", 
		Font="serif", 
		Position="0, -100", 
		Text="Hello World", 
		BackgroundColour="80, 80, 80, 255", 
		Background="Button1Brush", 
		Size="-100, 50", 
		AutoHeight="true"}, _scene)
	actor.set(label, "Text", "Hey man")

	-- Create XOML label from string
	local xoml = [[
	<Label Position="0, 100" Font="serif" Text="Test creating XOML resources from Lua" BackgroundColour="80, 80, 80, 255" Background="Button1Brush" Size="-100, -10" AutoHeight="true" IgnoreCamera="true" />]]
	resource.createFromString(xoml, _scene)

	return 1
end

	