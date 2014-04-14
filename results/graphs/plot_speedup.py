# Import everything from matplotlib (numpy is accessible via 'np' alias)
from pylab import *

# primes for which we ran tests on
primes = [101, 1009, 10007, 100003, 1000003, 10000019, 100000007, 1000000007]

# results from running our tests
results = [
    [1, 0.091668451, 0.041063034, 0.013804225, 0.008689120, 0.003375661],
    [1, 1.280046561, 0.591855041, 0.434908707, 0.240758986, 0.103387759],
    [1, 1.321793927, 1.480045819, 0.831154796, 0.519776011, 0.269167631],
    [1, 1.879523644, 2.994724977, 4.698312689, 4.574189533, 4.122256516],
    [1, 1.946645363, 3.300578549, 5.691183931, 6.171321822, 5.648115453],
    [1, 1.983700146, 3.440125059, 5.604951548, 7.714368484, 9.054070825],
    [1, 1.990384282, 3.600277010, 5.723019266, 7.640741198, 9.146780328],
    [1, 1.940726751, 3.822033452, 6.356447796, 8.350254990, 9.897219732]]

for n in xrange(0,8):
  fig = figure(figsize=(10, 10), dpi=80)
  g = fig.add_subplot(1,1,1)

  g.set_title('Speedup for n =' + str(primes[n]))
  g.set_xlabel('Number of processors')
  g.set_ylabel('Speedup')

  # Number of processors used
  processors = [1, 2, 4, 8, 16 ,32]
  # speed up values for each no. of processors
  speedup    = results[n]

  # Plot perfect speedup curve
  g.plot(processors, processors, color="red")

  #Plot actual speedup curve
  g.plot(processors, speedup)

  # Set the limits of the axis
  xlim(0, 31)
  ylim(0, 31)

  # Set x ticks and y ticks
  xticks(np.linspace(0,32,9,endpoint=True))
  yticks(np.linspace(0,32,9,endpoint=True))

  # Save figure using 72 dots per inch
  savefig(str(primes[n]) + '.png',dpi=72)

  # Show result on screen
  # show()
