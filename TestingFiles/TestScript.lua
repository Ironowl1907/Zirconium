-- This is called the when the entity is created
function OnInit()
    -- local entity = GetEntity()                        -- Now an Entity* in Lua
    -- local transform = entity:GetComponent_Transform() -- Access component
    -- transform.Translation.x = 5.0                     -- Modify component safely
    --

    v1 = vec3(1, 2, 3)
    v2 = vec3(4, 5, 6)

    v3 = v1 + v2
    print(v3.x, v3.y, v3.z) -- 5, 7, 9

    v4 = v1 * 2
    print(v4.x, v4.y, v4.z) -- 2, 4, 6
end

-- This is called every frame for the entity
function OnUpdate(dt)
    -- local entity = GetEntity()
    -- local sprite = entity:GetComponent_Sprite()
    -- sprite.Color = { 1, 0, 0, 1 } -- Change color to red
end
