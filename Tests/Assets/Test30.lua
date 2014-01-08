function KillJoint(_actor, _joint_name)

	local joint = physics.findJoint(_actor, _joint_name);
	physics.destroyJoint(_actor, joint);

	return 1;

end