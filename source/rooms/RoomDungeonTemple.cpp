/*
 *  Copyright (C) 2011-2015  OpenDungeons Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rooms/RoomDungeonTemple.h"

#include "game/Seat.h"
#include "gamemap/GameMap.h"
#include "entities/Creature.h"
#include "entities/PersistentObject.h"
#include "entities/ResearchEntity.h"
#include "entities/Tile.h"
#include "rooms/RoomManager.h"
#include "utils/LogManager.h"

static RoomManagerRegister<RoomDungeonTemple> reg(RoomType::dungeonTemple, "DungeonTemple");

RoomDungeonTemple::RoomDungeonTemple(GameMap* gameMap) :
    Room(gameMap),
    mTempleObject(nullptr)
{
    setMeshName("DungeonTemple");
}

void RoomDungeonTemple::updateActiveSpots()
{
    // Room::updateActiveSpots(); <<-- Disabled on purpose.
    // We don't update the active spots the same way as only the central tile is needed.
    if (getGameMap()->isInEditorMode())
        updateTemplePosition();
    else
    {
        if(mTempleObject == nullptr)
        {
            // We check if the temple already exists (that can happen if it has
            // been restored after restoring a saved game)
            if(mBuildingObjects.empty())
                updateTemplePosition();
            else
            {
                for(std::pair<Tile* const, RenderedMovableEntity*>& p : mBuildingObjects)
                {
                    if(p.second == nullptr)
                        continue;

                    // We take the first RenderedMovableEntity. Note that we cannot use
                    // the central tile because after saving a game, the central tile may
                    // not be the same if some tiles have been destroyed
                    mTempleObject = p.second;
                    break;
                }
            }
        }
    }
}

void RoomDungeonTemple::updateTemplePosition()
{
    // Only the server game map should load objects.
    if (!getIsOnServerMap())
        return;

    // Delete all previous rooms meshes and recreate a central one.
    removeAllBuildingObjects();
    mTempleObject = nullptr;

    Tile* centralTile = getCentralTile();
    if (centralTile == nullptr)
        return;

    mTempleObject = new PersistentObject(getGameMap(), true, getName(), "DungeonTempleObject", centralTile, 0.0, false);
    addBuildingObject(centralTile, mTempleObject);
}

void RoomDungeonTemple::destroyMeshLocal()
{
    Room::destroyMeshLocal();
    mTempleObject = nullptr;
}

bool RoomDungeonTemple::hasCarryEntitySpot(GameEntity* carriedEntity)
{
    if(carriedEntity->getObjectType() != GameEntityType::researchEntity)
        return false;

    // We accept any researchEntity
    return true;
}

Tile* RoomDungeonTemple::askSpotForCarriedEntity(GameEntity* carriedEntity)
{
    OD_ASSERT_TRUE_MSG(carriedEntity->getObjectType() == GameEntityType::researchEntity,
        "room=" + getName() + ", entity=" + carriedEntity->getName());
    if(carriedEntity->getObjectType() != GameEntityType::researchEntity)
        return nullptr;

    // We accept any researchEntity
    return getCentralTile();
}

void RoomDungeonTemple::notifyCarryingStateChanged(Creature* carrier, GameEntity* carriedEntity)
{
    OD_ASSERT_TRUE_MSG(carriedEntity->getObjectType() == GameEntityType::researchEntity,
        "room=" + getName() + ", entity=" + carriedEntity->getName());
    if(carriedEntity->getObjectType() != GameEntityType::researchEntity)
        return;

    // We check if the carrier is at the expected destination. If not on the wanted tile,
    // we don't accept the researchEntity
    // Note that if the wanted tile were to move during the transport, the researchEntity
    // will be dropped at its original destination and will become available again so there
    // should be no problem
    Tile* carrierTile = carrier->getPositionTile();
    if(carrierTile != getCentralTile())
        return;

    // We notify the player that the research is now available and we delete the researchEntity
    ResearchEntity* researchEntity = static_cast<ResearchEntity*>(carriedEntity);
    getSeat()->addResearch(researchEntity->getResearchType());
    researchEntity->removeFromGameMap();
    researchEntity->deleteYourself();
}

void RoomDungeonTemple::restoreInitialEntityState()
{
    // We need to use seats with vision before calling Room::restoreInitialEntityState
    // because it will empty the list
    if(mTempleObject == nullptr)
    {
        OD_ASSERT_TRUE_MSG(false, "roomDungeonTemple=" + getName());
        return;
    }

    Tile* tileTempleObject = mTempleObject->getPositionTile();
    if(tileTempleObject == nullptr)
    {
        OD_ASSERT_TRUE_MSG(false, "roomDungeonTemple=" + getName() + ", mTempleObject=" + mTempleObject->getName());
        return;
    }
    TileData* tileData = mTileData[tileTempleObject];
    if(tileData == nullptr)
    {
        OD_ASSERT_TRUE_MSG(false, "roomDungeonTemple=" + getName() + ", tile=" + Tile::displayAsString(tileTempleObject));
        return;
    }

    if(!tileData->mSeatsVision.empty())
        mTempleObject->notifySeatsWithVision(tileData->mSeatsVision);

    // If there are no covered tile, the temple object is not working
    if(numCoveredTiles() == 0)
        mTempleObject->notifyRemoveAsked();

    Room::restoreInitialEntityState();
}

int RoomDungeonTemple::getRoomCost(std::vector<Tile*>& tiles, GameMap* gameMap, RoomType type,
    int tileX1, int tileY1, int tileX2, int tileY2, Player* player)
{
    return getRoomCostDefault(tiles, gameMap, type, tileX1, tileY1, tileX2, tileY2, player);
}

void RoomDungeonTemple::buildRoom(GameMap* gameMap, const std::vector<Tile*>& tiles, Seat* seat)
{
    RoomDungeonTemple* room = new RoomDungeonTemple(gameMap);
    buildRoomDefault(gameMap, room, tiles, seat);
}

Room* RoomDungeonTemple::getRoomFromStream(GameMap* gameMap, std::istream& is)
{
    return new RoomDungeonTemple(gameMap);
}
