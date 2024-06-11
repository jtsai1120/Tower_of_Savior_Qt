# Project TOS
    This is a C++ OOP project using Qt to build a Tower of Savior game.

### Course
    Introduction to Computer Science 2 lectured by Prof. Yean-Ru Chen, in 2024 NCKUEE.

### Group 16
#### Member & Work Contribution
- **蔡承希**：Runestones, Runestones Drifting, Combo Calculation, Combo Eliminate, Drop Animation, Setting menu, Hp / Countdown bar
- **蔡辰鑫**：Enemy(CD, Hp bar), Attack Effect, Attack all
- **黃士洵**：Characters(Skills), Character selecting menu, Damage Calculation, Burned / Weathered Runestones and Burning trace

# Basic intro.

## Final Goal
    The game's final goal is to defeat the enemies by runestone drifting
- If the following happens, the game is over and will reset the game : 
    1. If your HP is below zero, you **Lose**.
    2. If you surrender, you **Lose**.
    4. If you pass all of the three levels, you **Win**.

## Mission 1 (Basic requirement)
### Choosing characters
    You can choose 6 characters at most, and the captain and the vice-captain must be selected.
- You can choose the character by left-clicking on the empty character slots.
- For faster selection, you can right-click, then five characters would be passed for each click.
- Whenever during the game, you can press the setting button on the upper right corner to see the detailed informaiton of different level (of the current mission).

### Runestone drifting
    Runestone drifting would be the main source of your attack power
- Select and left-click any runestone that you want to start from.
- Press and hold the left button and start drifting.
- While moving the selected runestone, it will switch the place with any runestone it encounters in the path.
- The goal is to make as many combos as possible (three of more identical runestones in a row or column)

#### Counting combos
    Attack power will only be counted if you make combos successfully
    
### Attack and attributes
    There are five attributes in this game, which will affect the final damage
    When the character counteract the enemy, double the original damage, when the character is counteracted by the enemy, half the original damage.
1. **Earth** : Counteract water, counteracted by fire.
2. **Fire** : Counteract earth, counteracted by water.
3. **Water** : Counteract fire, counteracted by earth.
4. **Dark** : Counteract light.
5. **Ligth** : Counteract dark.
- The enemy to attack will be randomly chosen, and the corresponding damage will be based on the attribute of the character and the chosen enemy.

### Enemy attack
    When the CD of each enemy turns 0, enemy will attack.
- Each enemy has its own attack power, it will attack you directly and has nothing to do with the characters you have chosen

### Next level
    After defeating every enemy in the current level, you will enter the next level (there are three levels in total)


# How to Compile & Run (for Cygwin)
In the game folder ( /game/ ) , use the following command :
```bash
$ make
```
Run:  
Open second terminal, use the following command :
```$ startxwin```

then back to the initial terminal and input :
```
$ export DISPLAY=:0.0
$ ./Project2_Group16
```


# Project Attainment
    What requirements and bonus we had done and implement in this project.

### Background Music (BGM) and Sound Effects
- We integrate the battle BGM, and the battle BGM may differ if you choose particular characters as captain.
    (e.g. Bocchi from Bocchi-the-Rock has the BGM - Guitar, Loneliness and Blue Planet. Enjoy it :) )
- When player eliminate different number of combo, different Sound Effect will be played.
- Settings Button, GameOver, GameWin all have different Sound Effects.

### Runestone Dropping Animation
- We have implemented very smooth dropping animation.

### Light-Halo-Vfxs
- We add the Light Visual Effect behind the eliminated runetones, which will show up, become bigger and then become smaller and end up disappear.

## Mission 2 (Bonus)
    In the following part, only the bonus parts which are not included in basic requirement will be listed.

### Basic setting
- In mission 2, the attack power of character will be higher. At the same time, the attack power of the enemy and its HP increases, too.
- CD of each enemy may be different from mission 1.
- Your HP will also be higher in mission 2.

### Choosing characters
- When choosing the characters, the space below the character slots shows the skill of the character.
- The captain trait is also listed. Namely, when the character is chosen to be captain, the captain trait will be activated.

### Character skills
- Whenever the skill is available, the position of the character will be a little higher than the original position, you can use the skill of particular character by clicking the picture of the character.
- After using the skill, you will not be able to use it again unless its skill CD turns 0.
- During the game, you will be able to check the information of the skill of each character by right-clicking on the character, the information will be showed on the runestone area, left-clicking at the runestone area after checking.
- We have implement some skills, some of which are very very hard to accomplish. for example, we have implement **additional attack, additional combos, freely sorting runestones(自由排珠), fixed pattern runestones, healing instead of been burnt while passing burning runestones, and many many more. Please enjoy our creativity :3**

### Attack all
- In mission 2, when you eliminate more than 5 runestones of a certain attribute a time, say, five water runestones in a row or column, then the water character will be able to attack every enemy that still survive at that moment.
  




