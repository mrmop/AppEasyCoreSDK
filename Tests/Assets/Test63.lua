local current_provider
local current_type
local web_view

function BuildHtml(ad_id, ad_type)
	local platform = sys.getDeviceType()

	if (current_provider == "inneractive") then
		return [[
			<html>
			<head>
				<meta name='viewport' content='width=device-width, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no' />
				<style type='text/css'>
					body {
						"margin:0;
					}
					#ad_align {
						width:100%25;
						text-align:center;
						text-align:-webkit-center;
					}
				</style>
			</head>
			<body>
 				<input id='iaAdtruthCollectorInput' type='hidden' value=''/>
				<script type='text/javascript' src='http://cdn2.inner-active.mobi/wv-js/adtruth.js'></script>
				<div id='iaAdPlaceholder'></div>
				<script>
					try {
						(function() {
							if (typeof ia === 'undefined') {
								ia = {};
							}
							ia.adSettings = {
								APP_ID:']] .. ad_id .. [[', 
								IS_MOBILE_WEB: false,
								IS_ORMMA_SUPPORT: false,
								IS_MRAID_SUPPORT: false,
								IS_INTERSTITIAL_AD: false,
								IS_FLOATING_BANNER: false,
								IMSI_MD5: '',
								IMSI_SHA1: '',
								ANDROID_ID_MD5: '',
								ANDROID_ID_SHA1: '',
								IDFA:'',
								IDFV:'',
								CATEGORY:'',
								AGE:'',
								GENDER:'',
								KEYWORDS: '',
								LOCATION:'',
								GPS_COORDINATES:'',
								DEVICE_WIDTH:'',
								DEVICE_HEIGHT:'',
								MOBILE_NETWORK_CODE:'',
								MOBILE_COUNTRY_CODE:'',
								NETWORK:'',
								REQUIRED_WIDTH:'',
								REQUIRED_HEIGHT:'',
								FAILOVER: ''
							};
							})();
					}
					catch (e) {
					}
				</script>
				<script src='http://cdn2.inner-active.mobi/wv-js/iaAdTagInternal.min.js' type='text/javascript'></script>
			</body>
		</html>]]
	elseif (current_provider == "leadbolt") then
		if (ad_type == "banner") then
			return [[
			<html>
				<head>
					<meta name="viewport" content="width=device-width, initial-scale=1.0, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no" />
				</head>
				<body style='margin:0; padding:0; text-align:center;'>
					<script type='text/javascript' src='http://ad.leadboltads.net/show_app_ad.js?section_id=]] .. ad_id .. [['></script>
				</body>
			</html>]]
		elseif (ad_type == "wall") then
			return [[
			<html>
				<head>
					<meta name="viewport" content="width=device-width, initial-scale=1.0, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no" />
				</head>
				<body style='margin:0; padding:0; text-align:center;'>
					<script type='text/javascript' src='http://ad.leadboltads.net/show_app_wall?section_id=]] .. ad_id .. [['></script>
				</body>
			</html>]]
		end
	elseif (current_provider == "admoda") then
		return [[
		<html>
			<head>
				<meta name='viewport' content='width=device-width, initial-scale=1.0, initial-scale=1.0, minimum-scale=1.0, maximum-scale=1.0, user-scalable=no' />
				<style> #iaAdContainerDiv { text-align: center; } </style>
			</head>
			<body style='margin:0;padding:0;text-align: center;'>
				<script src='http://www.admoda.com/ads/jsbanner.php?v=6&l=javascript&z=]] .. ad_id .. [['></script>
			</body>
		</html>]]
	end

	return nil
end

function ShowAd(_object, provider, type)
	print("ADS: Showing ad: " .. provider .. " - " .. type)

	-- If web view not created then create it
	if (web_view == nil) then
		local s = scene.find("Scene1")
		local temp = template.find("WebViewTemplate", s)
		template.from(temp, s)
		web_view = actor.find("AdsWebView", s)
	end

	current_provider = provider
	current_type = type
	local scene1 = scene.find("Scene1")
	local platform = sys.getDeviceType()

	actor.set(web_view, "visible", true)

	if (provider == "inneractive") then
		if (platform == "android") then
			actor.set(web_view, "html", BuildHtml("PocketeersLimited_BattleBallz_Android", type))
		elseif (platform == "iphone") then 
			actor.set(web_view, "html", BuildHtml("PocketeersLimited_BattleBallz_iPhone", type))
		elseif (platform == "ipad") then
			actor.set(web_view, "html", BuildHtml("PocketeersLimited_BattleBallz_iPad", type))
		else
			actor.set(web_view, "html", "Ads not supported")
		end
	elseif (provider == "leadbolt") then
		if (type == "banner") then
			actor.set(web_view, "html", BuildHtml("283012813", type))
		elseif (type == "wall") then
			actor.set(web_view, "URI", "http://ad.leadboltads.net/show_app_wall?section_id=780971284")
		end
	elseif (provider == "admoda") then
		if (type == "banner") then
			actor.set(web_view, "html", BuildHtml("11828", type))
		end
	end

	return 1;
end

function AdLoaded(_web_view)
	print("ADS: Loaded " .. actor.get(_web_view, "CurrentURI"))
	return 1
end

function AdLoading(_web_view)
	local uri = actor.get(_web_view, "CurrentURI")

	print("ADS: Loading " .. uri)

	if (current_provider == "leadbolt" and string.find(uri, "show_app_ad?") ~= nil) then
		-- This case is not a click
	elseif (current_type == "wall") then
		-- Prevents leadbolt app wall from opening in new browser window
	else
		if (string.find(uri, "http:") ~= nil) then
			print("ADS: Clicked Ad URL " .. uri)
			sys.launchURL(uri)
			ShowAd(nil, current_provider)
		end
	end

	return 1
end

function AdError(_web_view)
	print("ADS: Error " .. actor.get(_web_view, "CurrentURI"))
	return 1
end

