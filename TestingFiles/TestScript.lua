return {
    onCreate = function(this)
        print("Entity ID: ", this)
        print(GetSpriteColor(this).r)
        SetSpriteColor(this, 255, 0, 0, 1)
        print(GetSpriteColor(this).r)
    end,

    onUpdate = function(this, deltaTime)
        -- print("Updating entity with dt:", deltaTime, "FPS: ", 1 / deltaTime)
        -- local transform = GetTranslation(this);
        -- print(transform.x, transform.y, transform.z)
        -- SetTranslation(this, transform.x + deltaTime, 0, 0)
    end
}
