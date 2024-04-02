"""
@file         reset_data.py
@brief        This file is used to reset the data in the database to the default values.
              for resetting values for release
@date         290224
@author       Poh Jing Seng, jingseng.poh, (jingseng.poh@digipen.edu), 2301363

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
"""


import json


def constructData() -> dict:
    data = {
    "game": {
        "audio": {
            "sfx": 1,
            "music": 1
        },
        "difficulty": 0
    },
    "player": {
        "completedTutorial": false,
        "xpToNextLevel": 0,
        "level": 1,
        "inventory": [],
        "baseHealth": 100,
        "baseDamage": 100
    },
    "enemyAttributes": {
        "cat": {
            "texturePath": "./Assets/Combat_Enemy/cat.jpg",
            "soundPath": "./Assets/sounds/cat.mp3",
            "element": "fire",
            "health": 100,
            "damage": 10
        },
        "dragon": {
            "texturePath": "./Assets/Combat_Enemy/dragon.jpg",
            "soundPath": "./Assets/sounds/dragon.mp3",
            "element": "earth",
            "health": 150,
            "damage": 20
        },
        "horse": {
            "texturePath": "./Assets/Combat_Enemy/horse.jpg",
            "soundPath": "./Assets/sounds/horse.mp3",
            "element": "water",
            "health": 120,
            "damage": 12
        }
    },
    "items": {
        "bacon": {
            "texturePath": "./Assets/food/bacon.jpg",
            "healthEffect": 0,
            "dmgMultiplier": 1.15,
            "description": "Increases attack damage"
        },
        "beef": {
            "texturePath": "./Assets/food/beef.jpg",
            "healthEffect": 20,
            "dmgMultiplier": 1,
            "description": "Recovers 20 health"
        },
        "chicken": {
            "texturePath": "./Assets/food/chicken.jpg",
            "healthEffect": 0,
            "dmgMultiplier": 1.5,
            "description": "Increases attack damage greatly"
        }
    },
    "levels": [
    {
        "levelName": "TUTORIAL",
        "levelCompletionRate": 0.9,
        "unlocked": false,
        "completed": false,
        "maxEnemies": 1,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 2,
        "enemySpawnWeight": [
            {
                "cat": 2,
                "dog": 1
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 10,
                "Tree": 1,
                "Rock": 1
            }
        ],
        "DayTime": true
    },
    {
        "levelName": "Level 1: Plains",
        "levelCompletionRate": 0.9,
        "unlocked": false,
        "completed": false,
        "maxEnemies": 2,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 4,
        "enemySpawnWeight": [
            {
                "dog": 1
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 10,
                "Rock": 1
            }
        ],
        "DayTime": true
    },
    {
        "levelName": "Level 2: Rolling Plains",
        "levelCompletionRate": 0.9,
        "unlocked": false,
        "completed": false,
        "maxEnemies": 2,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 8,
        "enemySpawnWeight": [
            {
                "dog": 1
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 15,
                "Tree": 3
            }
        ],
        "DayTime": true
    },
    {
        "levelName": "Level 3: Forest",
        "levelCompletionRate": 0.9,
        "unlocked": false,
        "completed": false,
        "maxEnemies": 2,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 20,
        "enemySpawnWeight": [
            {
                "dog": 1
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 4,
                "Tree": 15
            }
        ],
        "DayTime": true
    },
    {
        "levelName": "Level 4: Forest Sunset",
        "levelCompletionRate": 0.9,
        "unlocked": false,
        "completed": false,
        "maxEnemies": 3,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 20,
        "enemySpawnWeight": [
            {
                "dog": 1
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 4,
                "Tree": 20
            }
        ],
        "DayTime": true
    },
    {
        "levelName": "Level 5: Forest - Night",
        "levelCompletionRate": 0.3,
        "unlocked": false,
        "completed": false,
        "maxEnemies": 4,
        "enemySpawnRate": 20,
        "maxSceneOBJperTile": 20,
        "enemySpawnWeight": [
            {
                "dog": 2
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 4,
                "Tree": 20
            }
        ],
        "DayTime": false
    },
    {
        "levelName": "Level 6: Rocky Plains",
        "levelCompletionRate": 0.9,
        "unlocked": false,
        "completed": false,
        "maxEnemies": 2,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 12,
        "enemySpawnWeight": [
            {
                "dog": 2
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Rock": 10
            }
        ],
        "DayTime": true
    },
    {
        "levelName": "Level 7: ??? - DANGER",
        "levelCompletionRate": 0.3,
        "unlocked": false,
        "completed": false,
        "maxEnemies": 4,
        "enemySpawnRate": 20,
        "maxSceneOBJperTile": 20,
        "enemySpawnWeight": [
            {
                "dog": 2
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Rock": 10
            }
        ],
        "DayTime": true
    }
]
}

    return data


def resetData():
    data = constructData()
    with open('data.json', 'w') as file:
        json.dump(data, file, indent=2)


def main():
    resetData()


if __name__ == "__main__":
    main()
