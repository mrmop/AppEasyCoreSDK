local Keys = 
{
	KeyNone							= 0, 
	KeyEsc 							= 1, 
	KeyTab 							= 2, 
	KeyBackspace 					= 3, 
	KeyEnter 						= 4, 
	KeyLeftShift 					= 5, 
	KeyLeftControl 					= 6, 
	KeyReserved 					= 7, 
	KeySpace 						= 8, 
	KeyLeft 						= 9, 
	KeyUp 							= 10, 
	KeyRight 						= 11, 
	KeyDown 						= 12, 
	Key0 							= 13, 
	Key1 							= 14, 
	Key2 							= 15, 
	Key3 							= 16, 
	Key4 							= 17, 
	Key5 							= 18, 
	Key6 							= 19, 
	Key7 							= 20, 
	Key8 							= 21, 
	Key9 							= 22, 
	KeyA 							= 23, 
	KeyB 							= 24, 
	KeyC 							= 25, 
	KeyD 							= 26, 
	KeyE 							= 27, 
	KeyF 							= 28, 
	KeyG 							= 29, 
	KeyH 							= 30, 
	KeyI 							= 31, 
	KeyJ 							= 32, 
	KeyK 							= 33, 
	KeyL 							= 34, 
	KeyM 							= 35, 
	KeyN 							= 36, 
	KeyO 							= 37, 
	KeyP 							= 38, 
	KeyQ 							= 39, 
	KeyR 							= 40, 
	KeyS 							= 41, 
	KeyT 							= 42, 
	KeyU 							= 43, 
	KeyV 							= 44, 
	KeyW 							= 45, 
	KeyX 							= 46, 
	KeyY 							= 47, 
	KeyZ 							= 48, 
	KeyF1 							= 49, 
	KeyF2 							= 50, 
	KeyF3 							= 51, 
	KeyF4 							= 52, 
	KeyF5 							= 53, 
	KeyF6 							= 54, 
	KeyF7 							= 55, 
	KeyF8 							= 56, 
	KeyF9 							= 57, 
	KeyF10 							= 58, 
	KeyNumPad0 						= 59, 
	KeyNumPad1 						= 60, 
	KeyNumPad2 						= 61, 
	KeyNumPad3 						= 62, 
	KeyNumPad4 						= 63, 
	KeyNumPad5 						= 64, 
	KeyNumPad6 						= 65, 
	KeyNumPad7 						= 66, 
	KeyNumPad8 						= 67, 
	KeyNumPad9 						= 68, 
	KeyNumPadPlus 					= 69, 
	KeyNumPadMinus 					= 70, 
	KeyNumPadEnter 					= 71, 
	KeyRighSoft 					= 72, 
	KeyLeftSoft 					= 73, 
	KeyLeftShoulder 				= 74, 
	KeyRightShoulder 				= 75, 
	KeyHash 						= 76, 
	KeyStar 						= 77, 
	KeyOk 							= 78, 
	KeyCLR 							= 79, 
	KeyPageUp 						= 80, 
	KeyPageDown 					= 81, 
	KeyCamera 						= 82, 
	KeyMicrophone 					= 83, 
	KeyFn 							= 84, 
	KeySym 							= 85, 
	KeyAccept 						= 86, 
	KeyEnd 							= 87, 
	KeyHomePage 					= 88, 
	KeyButton1 						= 89, 
	KeyButton2 						= 90, 
	KeyButton3 						= 91, 
	KeyButton4 						= 92, 
	KeyButton5 						= 93, 
	KeyButton6 						= 94, 
	KeyButton7 						= 95, 
	KeyButton8 						= 96, 
	KeyF11 							= 97, 
	KeyF12 							= 98, 
	KeyLeftAlt 						= 99, 
	KeyRightControl 				= 100, 
	KeyRightAlt 					= 101, 
	KeyRightShift 					= 102, 
	KeyBacktick 					= 103, 
	KeyComma 						= 104, 
	KeyPeriod 						= 105, 
	KeySlash 						= 106, 
	KeyBackSlash 					= 107, 
	KeySemicolon 					= 108, 
	KeyApostrophe 					= 109, 
	KeyLeftBracket 					= 110, 
	KeyRightBracket 				= 111, 
	KeyEquals 						= 112, 
	KeyMinus 						= 113, 
	KeyCapsLock 					= 114, 
	KeyNumPadPeriod 				= 115, 
	KeyNumPadSlash 					= 116, 
	KeyNumLock 						= 117, 
	KeyInsert 						= 118, 
	KeyHome 						= 119, 
	KeyPageUp 						= 120, 
	KeyPageDown 					= 121, 
	KeyEnd 							= 122, 
	KeyDelete 						= 123, 
	KeyPause 						= 124, 
	KeyAt 							= 125, 
	KeyBack 						= 126, 
	KeyMenu 						= 127, 
	KeySearch 						= 128, 
	Key3DMode 						= 129, 
	KeyMyApps						= 130, 
	KeyAbsGameA 					= 131, 
	KeyAbsGameB 					= 132, 
	KeyAbsGameC 					= 133, 
	KeyAbsGameD 					= 134, 
	KeyAbsUp 						= 135, 
	KeyAbsDown 						= 136, 
	KeyAbsLeft 						= 137, 
	KeyAbsRight 					= 138, 
	KeyAbsOk 						= 139, 
	KeyAbsASK 						= 140, 
	KeyAbsBSK 						= 141, 
}

local KeyNames = 
{
	"None",							-- 0
	"Esc", 							-- 1
	"Tab", 							-- 2
	"Backspace", 					-- 3
	"Enter", 						-- 4
	"LeftShift", 					-- 5
	"LeftControl", 					-- 6
	"Reserved", 					-- 7
	"Space", 						-- 8
	"Left", 						-- 9
	"Up", 							-- 10
	"Right", 						-- 11
	"Down", 						-- 12
	"0", 							-- 13
	"1", 							-- 14
	"2", 							-- 15
	"3", 							-- 16
	"4", 							-- 17
	"5", 							-- 18
	"6", 							-- 19
	"7", 							-- 20
	"8", 							-- 21
	"9", 							-- 22
	"A", 							-- 23
	"B", 							-- 24
	"C", 							-- 25
	"D", 							-- 26
	"E", 							-- 27
	"F", 							-- 28
	"G", 							-- 29
	"H", 							-- 30
	"I", 							-- 31
	"J", 							-- 32
	"K", 							-- 33
	"L", 							-- 34
	"M", 							-- 35
	"N", 							-- 36
	"O", 							-- 37
	"P", 							-- 38
	"Q", 							-- 39
	"R", 							-- 40
	"S", 							-- 41
	"T", 							-- 42
	"U", 							-- 43
	"V", 							-- 44
	"W", 							-- 45
	"X", 							-- 46
	"Y", 							-- 47
	"Z", 							-- 48
	"F1", 							-- 49
	"F2", 							-- 50
	"F3", 							-- 51
	"F4", 							-- 52
	"F5", 							-- 53
	"F6", 							-- 54
	"F7", 							-- 55
	"F8", 							-- 56
	"F9", 							-- 57
	"F10", 							-- 58
	"NumPad0", 						-- 59
	"NumPad1", 						-- 60
	"NumPad2", 						-- 61
	"NumPad3", 						-- 62
	"NumPad4", 						-- 63
	"NumPad5", 						-- 64
	"NumPad6", 						-- 65
	"NumPad7", 						-- 66
	"NumPad8", 						-- 67
	"NumPad9", 						-- 68
	"NumPadPlus", 					-- 69
	"NumPadMinus", 					-- 70
	"NumPadEnter", 					-- 71
	"RighSoft", 					-- 72
	"LeftSoft", 					-- 73
	"LeftShoulder", 				-- 74
	"RightShoulder", 				-- 75
	"Hash", 						-- 76
	"Star", 						-- 77
	"Ok", 							-- 78
	"CLR", 							-- 79
	"PageUp", 						-- 80
	"PageDown", 					-- 81
	"Camera", 						-- 82
	"Microphone", 					-- 83
	"Fn", 							-- 84
	"Sym", 							-- 85
	"Accept", 						-- 86
	"End", 							-- 87
	"HomePage", 					-- 88
	"Button1", 						-- 89
	"Button2", 						-- 90
	"Button3", 						-- 91
	"Button4", 						-- 92
	"Button5", 						-- 93
	"Button6", 						-- 94
	"Button7", 						-- 95
	"Button8", 						-- 96
	"F11", 							-- 97
	"F12", 							-- 98
	"LeftAlt", 						-- 99
	"RightControl", 				-- 100
	"RightAlt", 					-- 101
	"RightShift", 					-- 102
	"Backtick", 					-- 103
	"Comma", 						-- 104
	"Period", 						-- 105
	"Slash", 						-- 106
	"BackSlash", 					-- 107
	"Semicolon", 					-- 108
	"Apostrophe", 					-- 109
	"LeftBracket", 					-- 110
	"RightBracket", 				-- 111
	"Equals", 						-- 112
	"Minus", 						-- 113
	"CapsLock", 					-- 114
	"NumPadPeriod", 				-- 115
	"NumPadSlash", 					-- 116
	"NumLock", 						-- 117
	"Insert", 						-- 118
	"Home", 						-- 119
	"PageUp", 						-- 120
	"PageDown", 					-- 121
	"End", 							-- 122
	"Delete", 						-- 123
	"Pause", 						-- 124
	"At", 							-- 125
	"Back", 						-- 126
	"Menu", 						-- 127
	"Search", 						-- 128
	"3DMode", 						-- 129
	"MyApps",						-- 130
	"AbsGameA", 					-- 131
	"AbsGameB", 					-- 132
	"AbsGameC", 					-- 133
	"AbsGameD", 					-- 134
	"AbsUp", 						-- 135
	"AbsDown", 						-- 136
	"AbsLeft", 						-- 137
	"AbsRight", 					-- 138
	"AbsOk", 						-- 139
	"AbsASK", 						-- 140
	"AbsBSK", 						-- 141
}

function CheckForKeyPress(_scene)
	
	-- Get key pressed, returns 0 if none pressed
	local key = input.getKeyPressed()
	if (key ~= 0) then
		local key_var = variable.find("KeyPressed", _scene)
		variable.set(key_var, KeyNames[key + 1])
		local keycode_var = variable.find("KeyCode", _scene)
		variable.set(keycode_var, key)
	end

	-- Exit if space bar pressed
	if (input.isKeyDown(Keys.KeySpace)) then
		sys.exit()
	end

	-- Move with cursor keys
	local label = actor.find("Label1")
	if (input.isKeyDown(Keys.KeyLeft)) then
		actor.add(label, "PositionX", -2)
	end
	if (input.isKeyDown(Keys.KeyRight)) then
		actor.add(label, "PositionX", 2)
	end
	if (input.isKeyDown(Keys.KeyUp)) then
		actor.add(label, "PositionY", -2)
	end
	if (input.isKeyDown(Keys.KeyDown)) then
		actor.add(label, "PositionY", 2)
	end


	return 1
end

