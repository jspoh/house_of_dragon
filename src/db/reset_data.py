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
        "completedTutorial": False,
        "xpToNextLevel": 0,
        "level": 1,
        "inventory": [],
        "baseHealth": 100,
        "baseDamage": 100
    },
    "enemyAttributes": {
        "dragon": {
            "texturePath": "./Assets/Combat_Enemy/dragon.png",
            "soundPath": "./Assets/sounds/dragon.mp3",
            "element": "earth",
            "health": 150,
            "damage": 20
        },
        "horse": {
            "texturePath": "./Assets/Combat_Enemy/horse.png",
            "soundPath": "./Assets/sounds/horse.mp3",
            "element": "water",
            "health": 120,
            "damage": 12
        },
        "pig": {
                "texturePath": "./Assets/Combat_Enemy/pig.png",
                "soundPath": "./Assets/sounds/dragon.mp3",
                "element": "wood",
                "health": 200,
                "damage": 5
        },
        "snake": {
                "texturePath": "./Assets/Combat_Enemy/snake.png",
                "soundPath": "./Assets/sounds/dragon.mp3",
                "element": "metal",
                "health": 110,
                "damage": 15
        },
        "monkey": {
                "texturePath": "./Assets/Combat_Enemy/monkey.png",
                "soundPath": "./Assets/sounds/dragon.mp3",
                "element": "fire",
                "health": 110,
                "damage": 15
        },
        "goat": {
                "texturePath": "./Assets/Combat_Enemy/goat.png",
                "soundPath": "./Assets/sounds/dragon.mp3",
                "element": "earth",
                "health": 90,
                "damage": 25
        },
        "cow": {
                "texturePath": "./Assets/Combat_Enemy/cow.png",
                "soundPath": "./Assets/sounds/dragon.mp3",
                "element": "water",
                "health": 150,
                "damage": 25
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
        "unlocked": False,
        "completed": False,
        "maxEnemies": 1,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 2,
        "enemySpawnWeight": [
            {
                "cat": 2,
                "dragon": 1
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 10,
                "Tree": 1,
                "Rock": 1
            }
        ],
        "DayTime": True
    },
    {
        "levelName": "Level 1: Plains",
        "levelCompletionRate": 0.9,
        "unlocked": False,
        "completed": False,
        "maxEnemies": 2,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 4,
        "enemySpawnWeight": [
            {
                "dragon": 2,
                "snake" : 3

            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 10,
                "Rock": 1
            }
        ],
        "DayTime": True
    },
    {
        "levelName": "Level 2: Rolling Plains",
        "levelCompletionRate": 0.9,
        "unlocked": False,
        "completed": False,
        "maxEnemies": 2,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 8,
        "enemySpawnWeight": [
            {
                "dragon": 2
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 15,
                "Tree": 3
            }
        ],
        "DayTime": True
    },
    {
        "levelName": "Level 3: Forest",
        "levelCompletionRate": 0.9,
        "unlocked": False,
        "completed": False,
        "maxEnemies": 2,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 20,
        "enemySpawnWeight": [
            {
                "dragon": 2
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 4,
                "Tree": 15
            }
        ],
        "DayTime": True
    },
    {
        "levelName": "Level 4: Forest Sunset",
        "levelCompletionRate": 0.9,
        "unlocked": False,
        "completed": False,
        "maxEnemies": 3,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 20,
        "enemySpawnWeight": [
            {
                "dragon": 2
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 4,
                "Tree": 20
            }
        ],
        "DayTime": True
    },
    {
        "levelName": "Level 5: Forest - Night",
        "levelCompletionRate": 0.3,
        "unlocked": False,
        "completed": False,
        "maxEnemies": 4,
        "enemySpawnRate": 20,
        "maxSceneOBJperTile": 20,
        "enemySpawnWeight": [
            {
                "dragon": 2
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Grass": 4,
                "Tree": 20
            }
        ],
        "DayTime": False
    },
    {
        "levelName": "Level 6: Rocky Plains",
        "levelCompletionRate": 0.9,
        "unlocked": False,
        "completed": False,
        "maxEnemies": 2,
        "enemySpawnRate": 9,
        "maxSceneOBJperTile": 12,
        "enemySpawnWeight": [
            {
                "dragon": 2,
                "snake" : 3
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Rock": 20,
                "Grass": 2
            }
        ],
        "DayTime": True
    },
    {
        "levelName": "Level 7: ??? - DANGER",
        "levelCompletionRate": 0.3,
        "unlocked": False,
        "completed": False,
        "maxEnemies": 4,
        "enemySpawnRate": 20,
        "maxSceneOBJperTile": 30,
        "enemySpawnWeight": [
            {
                "dragon": 2
            }
        ],
        "SceneOBJSpawnWeight": [
            {
                "Rock": 10,
                "Grass": 2,
                "Tree": 2
            }
        ],
        "DayTime": True
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
