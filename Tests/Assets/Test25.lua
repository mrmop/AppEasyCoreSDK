function ShowJavascript(_actor)

	-- Display data that was sent back from Javascript
	local javascript_data = actor.get(_actor, "Javascript")
	print("javascript_data is " .. javascript_data)

	-- Execute some Javascript within the WebView actor
	actor.set(_actor, "Javascript", "document.getElementById('p1').innerHTML='We changed this paragraph from XOML';")

	return 1
end

