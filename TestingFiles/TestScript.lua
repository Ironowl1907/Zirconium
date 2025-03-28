-- This is called the when the entity is created
function OnInit()
    local entity = GetEntity()                        -- Now an Entity* in Lua
    local transform = entity:GetComponent_Transform() -- Access component
    print(transform)
    transform.Translation.x = 5.0                     -- Modify component safely
end

-- This is called every frame for the entity
function OnUpdate(dt)
    -- local entity = GetEntity()
    -- local sprite = entity:GetComponent_Sprite()
    -- sprite.Color = { 1, 0, 0, 1 } -- Change color to red
end
