function Test1(_object)
	-- Create a new memory based database
	local db = sqlite3.open_memory()

	-- Create a table and insetr some data
	db:exec[=[	CREATE TABLE users(name, age, cookies);
				INSERT INTO users VALUES("Mat", 31, 2);
				INSERT INTO users VALUES("Dave", 32, 3);
				INSERT INTO users VALUES("Sam", 33, 4);
			]=]

	-- Perform a select query
	local results = ""
	for row in db:nrows("SELECT * FROM users") do
		for i,v in pairs(row) do
			results = results .. tostring(i) .. " - " .. v .. "\n"
		end
	end

	print(results)

	-- Set results to Results label
	local sc = scene.find("Scene1")
	local label = actor.find("Results", sc)
	actor.set(label, "Text", results);

	return 1
end

