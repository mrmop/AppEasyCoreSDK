function StopTimer(_actor)
	local a = actor.find("Label1")
	local t = timer.find("Timer1", a)
	timer.stop(t)

	return 1
end

function RestartTimer(_actor)
	local a = actor.find("Label1")
	local t = timer.find("Timer1", a)
	timer.restart(t)

	return 1
end

