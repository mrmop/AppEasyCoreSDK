local purchase_token = nil

function PrintProp(prop, val)
	if (val ~= nil) then
		print(" - " .. prop .. " = " .. val)
	else
		print(" - " .. prop .. " = nil")
	end
end

function MarketCallback(event)
	print("************************ MarketCallback")
	if (event.event == "ready") then
		print("Market is ready")
	elseif (event.event == "error") then
		PrintProp("Market error", event.error)
	elseif (event.event == "unavailable") then
		print("Market is unavailable")
	elseif (event.event == "purchased") then
		print("Product purchased")
		PrintProp("productID", event.productID)
		PrintProp("restored", event.restored)
		PrintProp("payload", event.payload)
		PrintProp("date", event.date)
		PrintProp("subscriptionStartDate", event.subscriptionStartDate)
		PrintProp("subscriptionEndDate", event.subscriptionEndDate)
		PrintProp("purchaseToken", event.purchaseToken)
		PrintProp("transactionID", event.transactionID)
		purchase_token = event.transactionID
	elseif (event.event == "disabled") then
		print("Market is disabled")
	elseif (event.event == "info") then
		print("Product info is available")
		PrintProp("productID", event.productID)
		PrintProp("title", event.title)
		PrintProp("description", event.description)
		PrintProp("price", event.price)
	end

	local _scene = scene.find("Scene1")
	local _message = actor.find("Message", _scene)
	actor.set(_message, "Text", event.event)
end

function QueryProduct(_object)
	local product = market.find("coins10")
	market.query(product)
end

function MakePurchase(_object)
	local product = market.find("coins10")
	market.setPayload("Test payload")
	market.purchase(product)
end

function ConsumeProduct(_object)
	market.consume(purchase_token)
end

function RestorePurchases(_object)
	market.setCallback(MarketCallback)
	market.restore()
end

market.setCallback(MarketCallback)
market.setTestMode(true)
