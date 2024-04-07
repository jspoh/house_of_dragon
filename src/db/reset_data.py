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
    "difficulty": 1
  },
  "player": {
    "completedTutorial": true,
    "baseXpToLevelUp": 3,
    "xpPerEnemyKilled": 1,
    "currentXp": 0,
    "level": 1,
    "inventory": {
      "bacon": 3,
      "beef": 3,
      "chicken": 3
    },
    "baseHealth": 100,
    "baseDamage": 40
  },
  "enemyAttributes": {
    "bull": {
      "damage": 25,
      "element": "fire",
      "health": 150,
      "texturePath": "./Assets/Combat_Enemy/bull.png"
    },
    "chicken": {
      "damage": 25,
      "element": "wood",
      "health": 90,
      "texturePath": "./Assets/Combat_Enemy/chicken.png"
    },
    "dragon": {
      "damage": 30,
      "element": "metal",
      "health": 200,
      "texturePath": "./Assets/Combat_Enemy/dragon.png"
    },
    "goat": {
      "damage": 20,
      "element": "water",
      "health": 90,
      "texturePath": "./Assets/Combat_Enemy/goat.png"
    },
    "horse": {
      "damage": 25,
      "element": "water",
      "health": 120,
      "texturePath": "./Assets/Combat_Enemy/horse.png"
    },
    "monkey": {
      "damage": 20,
      "element": "wood",
      "health": 110,
      "texturePath": "./Assets/Combat_Enemy/monkey.png"
    },
    "pig": {
      "damage": 5,
      "element": "fire",
      "health": 200,
      "texturePath": "./Assets/Combat_Enemy/pig.png"
    },
    "rabbit": {
      "damage": 15,
      "element": "earth",
      "health": 90,
      "texturePath": "./Assets/Combat_Enemy/rabbit.png"
    },
    "snake": {
      "damage": 25,
      "element": "metal",
      "health": 110,
      "texturePath": "./Assets/Combat_Enemy/snake.png"
    }
  },
  "items": {
    "bacon": {
      "texturePath": "./Assets/Combat_Items/bacon.png",
      "healthEffect": 20,
      "dmgMultiplier": 1.15,
      "description": "Increases attack damage by a small amount and also recovers 20 health"
    },
    "beef": {
      "texturePath": "./Assets/Combat_Items/beef.png",
      "healthEffect": 40,
      "dmgMultiplier": 1,
      "description": "Recovers 40 health"
    },
    "chicken": {
      "texturePath": "./Assets/Combat_Items/chicken.png",
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
          "pig": 1
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
      "unlocked": true,
      "completed": false,
      "maxEnemies": 2,
      "enemySpawnRate": 9,
      "maxSceneOBJperTile": 4,
      "enemySpawnWeight": [
        {
          "rabbit": 2,
          "pig": 2,
          "goat": 1,
          "chicken": 2
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
          "rabbit": 1,
          "pig": 1,
          "goat": 1,
          "monkey": 1,
          "snake": 1,
          "chicken": 2
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
          "rabbit": 1,
          "pig": 1,
          "chicken": 1,
          "goat": 3,
          "monkey": 3,
          "snake": 3,
          "horse": 2,
          "bull": 2
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
          "rabbit": 1,
          "pig": 1,
          "chicken": 1,
          "goat": 2,
          "monkey": 2,
          "snake": 3,
          "horse": 3,
          "bull": 3
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
          "rabbit": 1,
          "pig": 1,
          "chicken": 1,
          "goat": 3,
          "monkey": 3,
          "snake": 3,
          "horse": 5,
          "bull": 5,
          "dragon": 2
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
          "rabbit": 1,
          "pig": 1,
          "chicken": 1,
          "goat": 1,
          "monkey": 1,
          "snake": 1,
          "horse": 5,
          "bull": 5,
          "dragon": 5
        }
      ],
      "SceneOBJSpawnWeight": [
        {
          "Rock": 20,
          "Grass": 2
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
      "maxSceneOBJperTile": 30,
      "enemySpawnWeight": [
        {
          "rabbit": 1,
          "pig": 1,
          "chicken": 1,
          "goat": 1,
          "monkey": 1,
          "snake": 1,
          "horse": 5,
          "bull": 5,
          "dragon": 10
        }
      ],
      "SceneOBJSpawnWeight": [
        {
          "Rock": 10,
          "Grass": 2,
          "Tree": 2
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
