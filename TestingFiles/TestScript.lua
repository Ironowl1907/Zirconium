-- This is called the when the entity is created
function OnInit()
end

-- This is called every frame for the entity
function OnUpdate(dt)
    local entity = GetEntityID()
    local tag = GetTagComponent(entity)
    print(tag.Tag)
end
