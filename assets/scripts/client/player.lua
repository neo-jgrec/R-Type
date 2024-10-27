function createPlayer(registry, assetManager, position, color, gameScale, playerId, isSelf)
    local player = registry:create_entity()

    -- Add components to player entity
    registry:add_component(player, TransformComponent.new(position, Vector2.new(33.0, 17.0), gameScale * 3.5, 0.0))

    local collisionComponent = CollisionComponent.new(PLAYER, { FloatRect.new(0.0, 0.0, 33.0, 17.0) })
    collisionComponent.onCollision = function(self, other)
        registry:remove_component(DrawableComponent, self)
        game:releaseColor(color)
    end
    registry:add_component(player, collisionComponent)

    registry:add_component(player, VelocityComponent.new(10.0, 10.0))
    if isSelf then
        registry:add_component(player, InputStateComponent.new())
    end
    registry:add_component(player, KeyBinding.new())
    registry:add_component(player, HealthComponent.new(10))
    registry:add_component(player, ScoreComponent.new(0))
    registry:add_component(player, Player.new(playerId, isSelf))
    registry:add_component(player, ShootCounterComponent.new(0))
    registry:add_component(player, PlayerColorComponent.new(color))

    -- Setting texture and animation
    local texture = assetManager:getTexture("player")
    local playerShape = RectangleShape.new(Vector2.new(33.0, 17.0))
    playerShape:setTexture(texture)
    playerShape:setTextureRect(IntRect.new(0, color * 17, 33, 17))

    registry:add_component(player, DrawableComponent.new(playerShape))
    registry:add_component(player, SceneComponent.new(GameState.Playing))
    registry:add_component(player, TextureComponent.new(texture))

    local moveFrames = {}
    for i = 0, 4 do
        table.insert(moveFrames, IntRect.new(i * 33, color * 17, 33, 17))
    end

    local animationComponent = AnimationComponent.new({
        animations = {
            [AnimationState.Moving] = moveFrames
        },
        frameTime = 0.1,
        elapsedTime = 0.0,
        currentFrame = 0,
        loop = false
    })

    registry:add_component(player, animationComponent)

    return player
end
