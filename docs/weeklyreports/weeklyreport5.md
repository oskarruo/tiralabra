# Weekly report, week 5

The program's main functionality is ready, so I mostly focused on documentation this week.
I added a user guide to the docs and a shortened version to the readme.
I also updated the testing report and implementation report.
I tried to find ways to improve the speed of the algorithms, and I found hashmap implementations faster than the standard C++ one, so I changed the algorithms to use the robin_hood::unordered_map. 
This improved the compression times of both algorithms.
Specifially the LZ78 compression got noticeably faster.

The project is essentially ready, so I am not sure what to do next. 
On week 2 I mentioned that I may add a third algorithm if I have time.
Unless it's a really simple algorithm, I am not sure if there is enough time to get one working and tested, as I have a lot of stuff to do in other courses.
I have working bit writing/reading classes ready, so it would probably take less time than the original two.
At the very least, I will continue looking into optimization of the current algorithms.
However, with my limited skills I doubt I can make any more large improvements. 

## Used hours

| Day | Time used | Description |
| ----- | ------------- | ------ |
| 10.4.  | 4h            | Doing peer review, optimization |
| 11.4.  | 3h            | Re-doing tests, writing documentation |
| Total | 7h         |        |