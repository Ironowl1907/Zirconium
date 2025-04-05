return {
    onCreate = function(this)
        print("Entity created with script:")
    end,

    onUpdate = function(this, deltaTime)
        print("Updating entity with dt:", deltaTime)
        local transform = GetTranslation(this);
        print(transform.x, transform.y, transform.z)
        SetTranslation(this, transform.x + deltaTime, 0, 0)
    end
}
