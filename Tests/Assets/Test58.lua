function MarketCallback(status, product_id)
	print("MarketCallback:")
	print("Status: " .. status)
	print("Product: " .. product_id)
end

function MakePurchase(_object)
	local product = market.find("coins1")
	market.setCallback(MarketCallback)
	market.purchase(product)
end