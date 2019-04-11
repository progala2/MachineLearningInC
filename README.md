# Machine Learning In C

It is a project for passing the Machine Learning subject in my College.

It covers Random Forest algorithm with many options to configure it, of course written in C as the name of the repository mentions it.

## System and Coding Env
Code is written with the help of the **VS2017/2019** IDE.

It is only compatible with **Windows 10/7** for now.

## First simple run

- You need to create a configuaration file first; Here is the most simple one:
```
TrainingFileName=data.csv
```

- Let's name it **config.cfg**. The *data.csv* stands for the path to the file with training data. **The TrainingFileName is mandatory** as it can't be changed during the run. (More about the config parameters [here](https://github.com/progala2/MachineLearningInC/wiki/Configs))

- Now when you run the program, you will be prompt to give a path to the config file.
- After that you have to choose if you want to normalize the data to [0,1] range or not. It cannot be changed during the run.
- If everything is ok with your data (it needs to be in [correct CSV format](https://github.com/progala2/MachineLearningInC/wiki/Data-format) for details) you will be prompt to write a [command](https://github.com/progala2/MachineLearningInC/wiki/Commands).
- Write *run* and hit the enter.
- Enjoy the results!
- write *save* to save the results
- write *exit* to exit :)

### Future?
If time allows:
- Playing with threads to make calculations faster
- Give even more plasticity for configuring the forest generation (various entropy equations, tree weighting etc.)
