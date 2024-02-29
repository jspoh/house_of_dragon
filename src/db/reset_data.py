"""
@file         reset_data.py
@brief        This file is used to reset the data in the database to the default values.
              for resetting values for release
@version      1.0
@date         290224
@author       jingseng.poh (jingseng.poh@digipen.edu)

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
        "sfx": 100,
        "music": 100
      }
    },
    "player": {
      "completedTutorial": False,
      "xpToNextLevel": 0,
      "level": 1,
      "inventory": []
    },
    "enemyAttributes": {
      "cat": {
        "texturePath": "./Assets/animals/cat.jpg",
        "soundPath": "./Assets/sounds/cat.mp3",
        "element": "fire",
        "health": 100,
        "damage": 10,
      }
    },
    "items": {
      "bacon": {
        "texturePath": "./Assets/food/bacon.jpg"
      },
      "beef": {
        "texturePath": "./Assets/food/beef.jpg"
      },
      "chicken": {
        "texturePath": "./Assets/food/chicken.jpg"
      }
    },
    "levels": [
      {
        "unlocked": True,
        "completed": False,
        "score":  0,
        "enemies": [
          {
            "type": "cat",
            "renderingPosition": [0, 0]
          }
        ]
      },
      {
        "unlocked": False,
        "completed": False,
        "score":  0,
        "enemies": [
          {
            "type": "cat",
            "renderingPosition": [0, 0]
          },
          {
            "type": "cat",
            "renderingPosition": [100, 100]
          }
        ]
      },
      {
        "unlocked": False,
        "completed": False,
        "score":  0,
        "enemies": [
          {
            "type": "cat",
            "renderingPosition": [0, 0]
          },
          {
            "type": "cat",
            "renderingPosition": [100, 100]
          },
          {
            "type": "cat",
            "renderingPosition": [-100, -100]
          }
        ]
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
