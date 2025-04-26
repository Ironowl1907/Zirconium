local function InputHandling(this, deltatime)
	if Input_IsKeyPressed(65) then
		print("A Pressed")
		RigidBodyComponent_ApplyLinearImpulseToCenter(this, -1, 0, true)
	elseif Input_IsKeyPressed(68) then
		print("D Pressed")
		RigidBodyComponent_ApplyLinearImpulseToCenter(this, 1, 0, true)
	elseif Input_IsKeyPressed(87) then
		print("W Pressed")
		RigidBodyComponent_ApplyLinearImpulseToCenter(this, 0, 1, true)
	end
end

return {
	onCreate = function(this)
		print("Init Script")
	end,
	onUpdate = function(this, deltaTime)
		InputHandling(this, deltaTime)
	end,
}
