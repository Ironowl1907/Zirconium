return {
    onCreate = function(this)

    end,

    onUpdate = function(this, deltaTime)
        local circleThickness = GetCircleThickness(this)
        SetCircleThickness(this, circleThickness - .2 * deltaTime)
    end
}
