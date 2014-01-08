local previousTime = sys.getTimeMs()
local gemsLeftToSpawn = 0
local canvasSize
local gemSpawnX
local gemSpawnY = -100
local maxGems = 33
local prevTouch = 0
local currentRound = 1
local roundScore = 0
local scoreMultiplier = 1
local gemDestroyedScore = 100
local targetGem = 0
local gemsInPlay = {0, 0, 0, 0, 0}
local paused = true
local maxGemTypes = 2

local gemBrushes = 
{
	"gem1", 
	"gem2", 
	"gem3", 
	"gem4", 
	"gem5", 
}
local gemShapes = 
{
	"gem1_shape", 
	"gem2_shape", 
	"gem3_shape", 
	"gem4_shape", 
	"gem5_shape", 
}

function newGame(_actor)
	-- Reset round
	currentRound = 1
	newRound(true)
	previousTime = sys.getTimeMs()

	return 1
end

function pauseGame(_actor)
	-- Scroll the menu in by setting its timeline to show the scroll_in animation 
	local pause_menu = scene.find("menu")
	local scroll_in = timeline.find("scroll_in", pause_menu)
	scene.set(pause_menu, "Timeline", scroll_in)
	timeline.restart(scroll_in)

	paused = true

	return 1
end

function continueGame(_actor)
	paused = false
	previousTime = sys.getTimeMs()

	return 1
end

function newRound(load_level)
	local game_scene = scene.find("game")

	-- Enable game scene
	scene.set(game_scene, "Active", true)

	-- Remove all game objects
	actor.destroyTagged("game", game_scene)

	-- If requested load new level
	if (load_level) then
		-- Remove environment shapes and geometries
		resource.destroyTagged("environment")
		-- Remove environment actors
		actor.destroyTagged("environment", game_scene)
		-- Load new level file
		sys.loadXoml("L" .. currentRound .. ".xml", game_scene)
	end

	-- Set up round
	paused = false
	scoreMultiplier = 1
	gemSpawnX = -(canvasSize.x * 0.8) / 2
	gemsLeftToSpawn = maxGems
	-- Reset gems in play counts
	for i=1,5 do
		gemsInPlay[i] = 0
	end
end

function endOfRound()
	paused = true
	local round_over_scene = scene.find("round_over")
	
	-- Set end of round score in UI
	local end_round_score = actor.find("end_round_score", round_over_scene)
	actor.set(end_round_score, "Text", roundScore)

	-- Score the end of round dialog into view
	local end_round_dialog = actor.find("end_round_dialog", round_over_scene)
	local round_over_in = timeline.find("round_over_in", round_over_scene)
	actor.set(end_round_dialog, "Timeline", round_over_in)
	timeline.restart(round_over_in)

	-- Disable game scene
	scene.set(scene.find("game"), "Active", false)

end

function nextRound(_actor)
	-- Move to next round
	currentRound = currentRound + 1
	-- Start new round
	newRound(true)
	return 1
end

function restartRound(_actor)
	-- Start new round
	newRound(false)
	return 1
end

function spawnPegs(_scene)
	local x = -(canvasSize.x * 0.7) / 2
	local y = 0
	local x_step = (-x * 2) / 5
	for i=0,5 do
		local peg = resource.create("Icon", {
				Type=6, 
				Position=tostring(x) .. "," .. tostring(y), 
				Background="peg", 
				Box2dMaterial="static", 
				Shape="peg_shape", 
				CollisionFlags="1,1,1",
				Tag="game"}, _scene)
		x = x + x_step
	end
	return 1
end

function spawnNewGem(_scene, _x, _y, _type)
	gemsInPlay[_type] = gemsInPlay[_type] + 1
	local gem = resource.create("Icon", {
			Type=_type, 
			Position=tostring(_x) .. "," .. tostring(_y), 
			Background=gemBrushes[_type], 
			Box2dMaterial="nobounce", 
			Shape=gemShapes[_type], 
			CollisionFlags="1,1,1",
			Tag="game"}, _scene)
end

function spawnNewBombGem(_scene, _x, _y, _type)
	local bomb_gem_temp = template.find("bomb_gem_temp")
	local params = {}
	params["type"] = tostring(_type + 32)
	params["pos"] = tostring(_x) .. "," .. tostring(_y)
	params["background"] = gemBrushes[_type]
	params["shape"] = gemShapes[_type]
	template.from(bomb_gem_temp, _scene, params)
end

function created(_scene)
	-- Calculate starting position of spawn
	canvasSize = scene.get(_scene, "CanvasSize")
	gemSpawnX = -(canvasSize.x * 0.8) / 2
	newRound()
	paused = true

	return 1
end

function tick(_scene)
	if (paused) then
		return 1
	end

	local time_ms = sys.getTimeMs()
	if (gemsLeftToSpawn > 0) then
		if ((time_ms - previousTime) > 100) then
			local spawn_width = (canvasSize.x * 0.8) / 2
			local gem_type = math.random(maxGemTypes)
			spawnNewGem(_scene, gemSpawnX, gemSpawnY, gem_type)
			previousTime = time_ms
			gemSpawnX = gemSpawnX + 75
			if (gemSpawnX > spawn_width) then
				gemSpawnX = -spawn_width
			end
			gemsLeftToSpawn = gemsLeftToSpawn - 1
			if (gemsLeftToSpawn == 0) then
				-- Spawn pegs after timer runs out
				timer.restart(timer.find("spawn_pegs", _scene))
			end
		end
	else
		if ((time_ms - previousTime) > 2000) then
			selectNewTargetGem()
			previousTime = time_ms
		else
			local touch_info = input.touchInfo(0)
			if (touch_info.z ~= 0 and prevTouch == 0 and touch_info.y < canvasSize.y / 2) then
				local scene_pos = scene.toScene(_scene, touch_info.x, touch_info.y, true)
				spawnNewBombGem(_scene, scene_pos.x, scene_pos.y, targetGem)
				selectNewTargetGem()
				previousTime = time_ms
			end
			prevTouch = touch_info.z
		end
	end

	return 1
end

function addToScore(_score)
	roundScore = roundScore + _score
	local score_label = actor.find("score", scene.find("game"))
	actor.set(score_label, "Text", roundScore)
end

function selectNewTargetGem()
	local available = {}
	for i=1,5 do
		if (gemsInPlay[i] > 0) then
			table.insert(available, i)
		end
	end

	if (#available == 0) then
		endOfRound()
		return
	end

	targetGem = available[math.random(#available)]
	local target_gem = actor.find("target_gem", scene.find("game"))
	actor.set(target_gem, "Background", gemBrushes[targetGem])
end

function createExplosion(_x, _y, _scale, _depth, _type)
	if (_type > 32) then _type = _type - 32 end
	-- Create an explosion actor from template
	local explosion_template = template.find("explosion_temp")
	local params = {}
	params["pos"] = tostring(_x) .. "," .. tostring(_y)
	params["scale"] = tostring(_scale)
	params["depth"] = tostring(_depth)
	params["image"] = gemBrushes[_type]
	template.from(explosion_template, scene.find("game"), params)
	
	-- Play explosion sound
--	local sound = resource.find("explosion", "sound");
--	local sound_instance = media.playSound(sound, false, 1, 1, 1);
end

function createExplosionHere(_actor)
	createExplosion(actor.get(_actor, "PositionX"), actor.get(_actor, "PositionY"), 1.0, actor.get(_actor, "Depth"), actor.get(_actor, "Type"))
	return 1
end

--
-- createScoreFloaterHere - Creates a score floater at the same position as the specified actor
-- 
function createScoreFloaterHere(_actor, _score)
	-- Create score floater from template
	local floater_template = template.find("score_floater_temp")
	local params = {}
	params["pos"] = tostring(actor.get(_actor, "PositionX")) .. "," .. tostring(actor.get(_actor, "PositionY"))
	params["depth"] = actor.get(_actor, "Depth")
	params["score"] = tostring(_score)
	template.from(floater_template, scene.find("game"), params)
	return 1
end

function destroyGem(_bomb)
	-- Create an explosion at same pos as bomb actor
	createExplosionHere(_bomb)

	local type = actor.get(_bomb, "type")
	if (type < 32) then
		-- Create score floater at same pos as bomb actor
		createScoreFloaterHere(_bomb, gemDestroyedScore * scoreMultiplier)
		-- Add to score
		addToScore(gemDestroyedScore * scoreMultiplier)
		-- Decrearse gem count
		gemsInPlay[type] = gemsInPlay[type] - 1
	end

	-- Destroy this bomb actor
	actor.destroy(_bomb)
end

function updateBombGem(_bomb)
	local user_props = actor.get(_bomb, "UserProperties")
	local hit_time = userprops.get(userprops.find(user_props, "HitTime"))
	if (hit_time == 0) then
		-- Not yet hit
	else
		-- Bomb hit something
		local time_diff = sys.getTimeMs() - hit_time
		if (time_diff > 2000) then
			-- Bomb has rolled for too long so destroy it
			destroyGem(_bomb)
			-- Reset multiplier
			scoreMultiplier = 1
		end
	end

	return 1
end

function bombGemCollisionStarted(_bomb)
	-- Get list of collisions that have just started
	local collisions_started = actor.getStartContacts(_bomb);
	if (collisions_started == nil) then
		return 1;
	end
	local btype = actor.get(_bomb, "Type") - 32

	local hit = false
	for key,collider in ipairs(collisions_started) do
		local type = actor.get(collider, "Type")
		if (type > 32) then type = type - 32 end
		if (btype == type) then
			destroyGem(collider)
			hit = true
			scoreMultiplier = scoreMultiplier + 1
		end
	end	
	if (hit == true) then
		local user_props = actor.get(_bomb, "UserProperties")
		local hit_time_prop = userprops.find(user_props, "HitTime")
		userprops.set(hit_time_prop, sys.getTimeMs())
	end
	
	return 1;
end

