found_letters = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
guesses_left = 5;
game_scene = nil;
total_letters = 0;
letters_found = 0;
message = "ZAAPZKL EK ZOPKICP";
answers = "APPEASYISAWESOME";

function InitGame(_parent)

	game_scene = _parent;
	print("answer:");
	print(answers);
	
	-- Generate letters
	-- Find character template
	char_template = template.find("CharacterTemp", _parent);
	
	local count = string.len(message);
	local start_x = -45;
	local x = start_x;
	local y = -30;
	
	for i = 1, count do

		local char = string.sub(message, i, i);
		if (char ~= " ") then
		
			-- Create template parameters
			local params = {};
			params["name"] = "L" .. tostring(total_letters);
			params["posx"] = tostring(x);
			params["posy1"] = tostring(y);
			params["posy2"] = tostring(y + 10);
			params["letter"] = char;
			print("char = " .. char);
			
			-- Instantiate the template
			template.from(char_template, _parent, params);
			found_letters[total_letters + 1] = 0;
			total_letters = total_letters + 1;
			x = x + 8;
		else
			if (x ~= start_x) then
				x = x + 8;
			end
		end
		
		-- Wrap text at edge of screen
		if (x >= 45) then
			x = start_x;
			y = y + 20;
		end
	end
	
	return 1;
end


function ResetGame(_parent)

	guesses_left = 5;
	letters_found = 0;

	-- Update guesses left label
	local label = actor.find("AttemptsLeft", game_scene);
	actor.set(label, "Text", string.format("%d", guesses_left));
	
	-- Reset letters found array
	for i=1, total_letters do
		found_letters[i] = 0
	end
	
	-- Reset the letter actors
	for i=1, total_letters do
		local name = "L" .. i - 1;
		local label = actor.find(name, game_scene);
		actor.set(label, "Text", "_");
		actor.set(label, "Tappable", true);
	end
	
	-- Start game over menu out animation
	local menu = actor.find("GameOverMenu", game_scene)
	actor.set(menu, "Timeline", "menu_anim_out");
	local tl = timeline.find("menu_anim_out", menu);
	timeline.restart(tl);
	
	return 1;
end

function GameOver()

	-- Show the game over menu
	local menu = actor.find("GameOverMenu", game_scene)
	actor.set(menu, "Timeline", "menu_anim_in");
	local tl = timeline.find("menu_anim_in", menu);
	timeline.restart(tl);

	return 1;
end

function GameComplete()

	-- Show the game complete  menu
	local menu = actor.find("GameCompleteMenu", game_scene)
	actor.set(menu, "Timeline", "menu_anim_in");
	local tl = timeline.find("menu_anim_in", menu);
	timeline.restart(tl);

	return 1;
end

function Guess(_parent)

	-- Ask user for a character
	local s = input.textInput("Guess a letter");
	if (s == nil or s == "") then 
		return 0;
	end
	s = string.upper(s);
	
	-- Get the label index from the parents name
	local name = actor.get(_parent, "Name");
	local index = tonumber(string.sub(name, 2)) + 1;
	
	-- Check to see if the entered character matches the hidden character at the same position
	print("answers=" .. answers);
	local g = string.sub(answers, index, index);
	if (g == s) then
		
		-- Update the labels showing any that match
		for i = 1, total_letters do
			local char = string.sub(answers, i, i);
			
			if (char == s and found_letters[i] == 0) then
				local name = "L" .. i - 1;
				local label = actor.find(name, game_scene);
				actor.set(label, "Text", s);
				actor.set(label, "Tappable", false);
				letters_found = letters_found + 1;
				found_letters[i] = 1;
				if (letters_found == total_letters) then
					GameComplete();
				end 
			end
		end
	else
		-- Update guesses left
		guesses_left = guesses_left - 1;
		if (guesses_left <= 0) then
			GameOver();
		else
			-- Update guesses left label
			local label = actor.find("AttemptsLeft", game_scene);
			actor.set(label, "Text", string.format("%d", guesses_left));
		end
	end

	return 1;
end

