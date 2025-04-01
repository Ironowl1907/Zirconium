-- This is called the when the entity is created
function OnInit()
end

-- This is called every frame for the entity
function OnUpdate(dt)
    local entity = GetEntity()
    local id = entity:GetID()
    local tag = GetTagComponent(id)
    print("Tag: " .. tag.Tag)
end
