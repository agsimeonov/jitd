package jitd;

import org.apache.logging.log4j.Logger;

public class Driver {
  
  private static Logger log = 
    org.apache.logging.log4j.LogManager.getLogger();

  public Cog root;
  public Mode mode;
  
  public Driver(Mode mode, Cog root)
  {
    this.root = root;
    this.mode = mode;
  }

  public KeyValueIterator scan(long low, long high)
  {
    return mode.scan(this, low, high);
  }

  public void insert(Cog values)
  {
    mode.insert(this, values);
  }
  public void idle() 
  {
    mode.idle(this);
  }
  public void dump()
  {
    System.out.println(root.toString());
  }
  
  /////////// EVALUATION CODE /////////////
  
  public static void usage()
  {
    System.out.println("Usage: jitd [options]");
    System.out.println("  --size [dataSize]   The number of records to bootstrap with");
    System.out.println("  --read [keyRange]   The number of keys to read with each op");
    System.out.println("  --write [dataSize]  The number of records to write with each op");
    System.out.println("  --ratio [readRatio] The percent (/100) of operations to read");
    System.out.println("  --ops [opCount]     The number of operations to perform");
    System.out.println("  --dump              Dump the final JITD state");
    System.out.println("  --fullScan          Perform a full k/v scan over read records");
    System.out.println("  --mode [mode]       Set mode to { naive, cracker, merge }");
  }
  
  public static void main(String argList[])
  {
    ArrayCog c;
    KeyValueIterator.RandomIterator rand;
    Mode mode;
    long startTime;
    long endTime;
    double totalTime;
    
    c = new ArrayCog(1000000);
    rand = new KeyValueIterator.RandomIterator();
    c.load(rand);
    mode = new CrackerMode();
    startTime = System.nanoTime();
    ((CrackerMode) mode).crackOne(c, 500000);
    endTime   = System.nanoTime();
    totalTime = ((double)endTime - startTime) / 1000000;
    System.out.println(totalTime);
    
    c = new ArrayCog(1000000);
    rand = new KeyValueIterator.RandomIterator();
    c.load(rand);
    startTime = System.nanoTime();
    ((CrackerMode) mode).crack(c, 333333, 666666);
    endTime   = System.nanoTime();
    totalTime = ((double)endTime - startTime) / 1000000;
    System.out.println(totalTime);
    
    c = new ArrayCog(1000000);
    rand = new KeyValueIterator.RandomIterator();
    c.load(rand);
    mode = new PushdownMergeMode();
    startTime = System.nanoTime();
    ((PushdownMergeMode) mode).amerge(c, 333333, 666666);
    endTime   = System.nanoTime();
    totalTime = ((double)endTime - startTime) / 1000000;
    System.out.println(totalTime);
  }
}