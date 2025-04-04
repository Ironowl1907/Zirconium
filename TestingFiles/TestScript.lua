return {
    onCreate = function(self)
        print("Entity created with script:", self)
    end,

    onUpdate = function(self, deltaTime)
        print("Updating entity with dt:", deltaTime)
    end
}
