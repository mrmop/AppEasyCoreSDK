json = require("json")

local FacebookAboutMeLoaded = false
local FacebookFriendListLoaded = false
local UserID = ""

function PrintTable(tbl, indent)
	if not indent then indent = 0 end
	for k, v in pairs(tbl) do
		formatting = string.rep("  ", indent) .. k .. ": "
		if type(v) == "table" then
			print(formatting)
			PrintTable(v, indent+1)
		elseif type(v) == 'boolean' then
			print(formatting .. tostring(v))		
		else
			print(formatting .. v)
		end
	end
end

function FacebookResponseDebugCallback(request, event)
	if (event == "response") then
		local response = facebook.response(request)
		if (response ~= nil) then
			local o = json.decode(response)
			print("type is " .. type(o))
			if (type(o) == "table") then
				PrintTable(o, 1)
			else
				print("response: " .. o)
			end
		end
	end
end

function FacebookGetFriendsListCallback(request, event)
	if (event == "response") then
		local response = facebook.response(request)
		if (response ~= nil) then
			-- Decode the JSON response from Facebook
			local friends = json.decode(response)
			-- Find the app scene, this is where we will create a list box to hold the friends list
			local _scene = scene.find("scene1")
			-- Create a label for each friend
			local list_box = actor.find("FriendsList", _scene)
			for k, v in pairs(friends.data) do
				local label = resource.create("Label", {
					Name=v.id,
					Font="serif", 
					Text=v.name, 
					BackgroundColour="80, 80, 80, 255", 
					Background="Button1Brush", 
					Size="-100, 60", 
					AutoHeight="false"}, list_box)
			end
			FriendListLoaded = true
			actor.set(actor.find("FriendsListPanel", _scene), "Visible", true)
		end
	end
end

function FacebookGetAboutMeCallback(request, event)
--	FacebookResponseDebugCallback(request, event)

	if (event == "response") then
		local response = facebook.response(request)
		if (response ~= nil) then
			-- Decode the JSON response from Facebook
			local about_me = json.decode(response)
			-- Find the app scene, this is where the controls live that we want to update
			local _scene = scene.find("scene1")
			-- Get name and assign to about me name label
			actor.set(actor.find("MyName", _scene), "Text", "Name: ".. about_me.name)
			-- Get user id and assign to about me name label
			actor.set(actor.find("MyID", _scene), "Text", "ID: ".. about_me.id)
			-- Show the about me panel
			actor.set(actor.find("AboutMePanel", _scene), "Visible", true)
			-- To get the users profile picture we need to pass the direct graph url request into an image
			local my_image = resource.find("MyImage", "Image", _scene)
			image.reload(my_image, "https://graph.facebook.com/" .. about_me.id .. "/picture?access_token=" .. facebook.token(), false)
			FacebookAboutMeLoaded = true
		end
	end
end

function FacebookLoginCallback(request, event)
	if (event == "loggedin") then
		local post_wall_button = actor.find("PostButton", scene.find("scene1"))
		actor.set(post_wall_button, "Visible", true)
		local about_me_button = actor.find("AboutMeButton", scene.find("scene1"))
		actor.set(about_me_button, "Visible", true)
		local friends_button = actor.find("FriendsButton", scene.find("scene1"))
		actor.set(friends_button, "Visible", true)
	end
end

function FacebookLogin(_actor)
	facebook.setCallback(FacebookLoginCallback)
	facebook.login({"read_stream", "publish_stream"}, "add_your_own_facebook_app_id_here")
	return 1
end

function FacebookFriends(_actor)
	-- Show the friends list panel and hide the about me panel
	actor.set(actor.find("FriendsListPanel", scene.find("scene1")), "Visible", true)
	actor.set(actor.find("AboutMePanel", scene.find("scene1")), "Visible", false)
	if (FriendListLoaded) then return 1 end
	-- Get users friends list Facebook
	facebook.setCallback(FacebookGetFriendsListCallback)
	facebook.graph("me/friends", "GET")
	return 1
end

function FacebookPostToWall(_actor)
	local data = {}
	data["message"] = "Test"
--	facebook.request("facebook.stream.publish", "POST", data)
	facebook.graph("me/feed", "POST", data)
	return 1
end

function FacebookAboutMe(_actor)
	-- Show the about me panel and hide friends list panel
	actor.set(actor.find("AboutMePanel", scene.find("scene1")), "Visible", true)
	actor.set(actor.find("FriendsListPanel", scene.find("scene1")), "Visible", false)
	if (FacebookAboutMeLoaded) then return 1 end
	-- Get about me info from Facebook
	facebook.setCallback(FacebookGetAboutMeCallback)
	facebook.graph("me", "GET")
	return 1
end

