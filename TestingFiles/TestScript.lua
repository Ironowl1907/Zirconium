-- This is called the when the entity is created
function OnInit()
    local entity = GetEntity()
end

-- This is called every frame for the entity
function OnUpdate(dt)
    local entity = GetEntity()
    if entity then
        local transform = entity:GetComponent_Transform()
        if transform then
            print(transform) -- This should be safe if transform exists
        else
            print("Transform component not found.")
        end
    else
        print("Entity pointer is null.")
    end
end
