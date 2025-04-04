return {
    onCreate = function(this)
        print("Entity created with script:")
    end,

    onUpdate = function(this, deltaTime)
        print("Updating entity with dt:", deltaTime)
    end
}
