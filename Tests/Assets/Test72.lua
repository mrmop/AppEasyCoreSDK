function adsAvailable(event, data1, data2)
	print("---------- ADS EVENT - " .. event .. ", " .. data1 .. ", " .. data2)
end

if (sys.getDeviceType() == "android") then
	ads.init("flurry_appspot", "add_your_id")
--	ads.init("revmob", "add_your_id")
else
	ads.init("flurry_appspot", "add_your_id")
--	ads.init("revmob", "add_your_id")
end
ads.setCallback(adsAvailable)

function ShowAd(_object, provider, type)
	print("ADS: Showing ad: " .. provider .. " - " .. type)
	
	if (provider == "chartboost") then
		if (type == "int") then
			ads.setProvider("chartboost", "inter")
			ads.loadShow("inter", "shuffle_match_full", 1)
		else
			ads.setProvider("chartboost", "moreapps")
			ads.loadShow("moreapps", "", 1)
		end
	elseif (provider == "revmob") then
		if (type == "int") then
			ads.setProvider("revmob", "inter")
			ads.loadShow("inter", "shuffle_match_full", 1)
		else
			ads.setProvider("revmob", "banner")
			ads.loadShow("banner", "shuffle_match_banner_top", 1)
		end
	elseif (provider == "appspot") then
		if (type == "int") then
			ads.setProvider("flurry_appspot", "inter")
			ads.loadShow("inter", "shuffle_match_full", 1)
		else
			ads.setProvider("flurry_appspot", "banner")
			ads.loadShow("banner", "shuffle_match_banner_top", 1)
		end

	end

	return 1;
end