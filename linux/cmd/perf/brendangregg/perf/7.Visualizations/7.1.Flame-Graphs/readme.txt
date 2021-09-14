Flame Graphs

CPU Flame Graph
Flame graphs are a visualization of profiled software, allowing the most frequent code-paths to be identified quickly and accurately. They can be generated using my open source programs on github.com/brendangregg/FlameGraph, which create interactive SVGs. See the Updates section for other implementations. Recently I've been helping with d3-flame-graph.

The following pages (or posts) introduce different types of flame graphs:

CPU
Memory
Off-CPU
Hot/Cold
Differential
The example on the right is a portion of a CPU flame graph, showing MySQL codepaths that are consuming CPU cycles, and by how much.

Summary
The x-axis shows the stack profile population, sorted alphabetically (it is not the passage of time), and the y-axis shows stack depth, counting from zero at the bottom. Each rectangle represents a stack frame. The wider a frame is is, the more often it was present in the stacks. The top edge shows what is on-CPU, and beneath it is its ancestry. The colors are usually not significant, picked randomly to differentiate frames.

This visualization is fully explained in my ACMQ article The Flame Graph, also published in Communications of the ACM, Vol. 59 No. 6.

Also see my CPU Flame Graphs page, and the presentation below.

Operating Systems
Flame graphs can be generated from any profile data that contains stack traces, including from the following profiling tools:

Linux: perf, eBPF, SystemTap, and ktap
Solaris, illumos, FreeBSD: DTrace
Mac OS X: DTrace and Instruments
Windows: Xperf.exe
Once you have a profiler that can generate meaningful stacks, converting them into a flame graph is usually the easy step.

Presentation
I gave an updated talk explaining flame graphs at USENIX ATC 2017 titled Visualizing Performance with Flame Graphs, which is on youtube and slideshare (PDF)

 

My first talk on flame graphs was at USENIX LISA 2013, which ended up as a plenary talk (youtube, slideshare, PDF):

Variations
Icicle charts are flame graphs upside down. Some people prefer it that way. My flamegraph.pl creates them using --inverted. I prefer the standard "flame" layout, where the y-axis is counting stack depth upwards from zero at the bottom. With icicle charts, the y-axis has zero at the top, and counts downwards, which I find odd (I'm used to line charts with a 0,0 origin in the bottom left). I'm also used to scanning them top-down to look for plateaus. But other developers are always reading root-to-leaf, and the icicle layout (with a GUI that starts at the top) means their starting point is always on the screen, and they don't need to scroll first.

Flame charts were first added by Google Chrome's WebKit Web Inspector (bug). While inspired by flame graphs, flame charts put the passage of time on the x-axis instead of the alphabet. This means that time-based patterns can studied. Flame graphs reorder the x-axis samples alphabetically, which maximizes frame merging, and better shows the big picture of the profile. Multi-threaded applications can't be shown sensibly by a single flame chart, whereas they can with a flame graphs (a problem flame charts didn't need to deal with, since it was initially used for single-threaded JavaScript analysis). Both visualizations are useful, and tools should make both available if possible (e.g., TraceCompass does). Some analysis tools have implemented flame charts and mistakingly called them flame graphs.

Sunburst layout using radial coordinates for the x-axis, a flame graph can be turned into a hierarchical pie chart. The Google Web Inspector team prototyped them.

Origin
I invented flame graphs when working on a MySQL performance issue and needed to understand CPU usage quickly and in depth. The regular profilers/tracers had produced walls of text, so I was exploring visualizations. I first traced CPU function calls and visualized it using Neelakanth Nadgir's time-ordered visualization for callstacks, which itself was inspired by Roch Bourbonnais's CallStackAnalyzer and Jan Boerhout's vftrace. These look similar to flame graphs, but have the passage of time on the x-axis. But there were two problems: the overhead of function tracing was too high, perturbing the target, and the final visualization was too dense to read when spanning multiple seconds. I switched to timed sampling (profiling) to solve the overhead problem, but since the function flow is no longer known (sampling has gaps) I ditched time on the x-axis and reordered samples to maximize frame merging. It worked, the final visualization was much more readable. Neelakanth and Roch's visualizations used completely random colors to differentiate frames. I thought it looked nicer to narrow the color palette, and picked just warm colors initially as it explained why the CPUs were "hot" (busy). Since it resembled flames, it quickly became known as flame graphs.

I described more detail of the original performance problem that led to flame graphs in my ACMQ/CACM article (link above). The flame graph visualization is really an adjacency diagram with an inverted icicle layout, which I used to visualize profiled stack traces.

Updates
Flame graphs were released in Dec 2011. Not long afterwards (updated in 2012):

Alan Coopersmith generated flame graphs of the X server.
Dave Pacheco created them with node.js functions.
Max Bruning has also shown how he used it to solve an IP scaling issue.
Dave Pacheco has also created stackvis, a Node.js implementation, available on npm.
Zoltan Farkas was inspired by flame graphs to create Spf4j (Simple performance framework for java), which includes them in a GUI.
More Flame Graph news (updated Apr 2013):

I wrote some documentation for Linux Kernel Flame Graphs, generated using either perf and SystemTap for the profile data.
Mark Probst developed Flame Graphs for Instruments on Mac OS X.
Sam Saffron has developed Flame graphs in Ruby MiniProfiler, and shows examples of amazingly deep stacks.
Bruce Dawson has an excellent post on Summarizing Xperf CPU Usage with Flame Graphs on Microsoft Windows. It includes examples for Visual Studio and Outlook, and a stack folding script to process the Xperf output.
Google Chrome's performance analysis tool, WebKit Web Inspector, introduced "Flame Charts", inspired by flame graphs. These are a similar visualization, but the x-axis is time, instead of the alphabet. Check out the screenshot, which includes a mouse-over popup that links to the source code. (Update: there is a bug to add flame graphs to Chrome, in addition to flame charts: Chromium 452624.)
Tim Bunce has been improving and adding features to Flame Graphs, and has included them in Perl's best profiler, Devel:::NYTProf, for profiling Perl. See his post on NYTProf v5 – Flaming Precision.
More Flame Graph news (updated Aug 2013):

I wrote a document summarizing four techniques for generating Memory Leak (and Growth) Flame Graphs, which visualize stacks with byte counts, instead of the traditional CPU sample Flame Graphs. I also colored them green to indicate that they are a different type.
John Graham-Cumming showed how CloudFlare was using SystemTap-generated flame graphs for optimizing their Lua WAF.
Yichun Zhang showed how Off-CPU Time Flame Graphs (PDF) can solve issues of blocking time. See Off-CPU Analysis for why this is important.
Igor Soarez wrote How To Make Flame Graphs for node.js analysis, showing all steps involved.
Paul Irish and Umar Hansa posted an awesome demo of using FlameCharts to investigate time in V8, which includes zooming in and clicking on functions to go to code, and Addy Osmani posted a longer video tutorial. While these aren't Flame Graphs, they show developments in a related visualization: a time-series version that retains sequence and ordering.
More Flame Graph news (updated Jun 2014):

I gave a talk at USENIX/LISA13 titled Blazing Performance with Flame Graphs, which covered them in enormous detail. See the slides and video. At the last minute this talk became a Plenary when another speaker became ill and had to cancel.
Joab Jackson wrote an article about flame graphs for PCWorld, InfoWorld, and CIO.
I wrote documents on Off-CPU Flame Graphs, and Hot/Cold Flame Graphs, based on what I had documented earlier for my USENIX talk.
I wrote a post showing how the Google lightweight-java-profiler can be used to create Java Flame Graphs. This is only Java code time, and not other CPU consumers. See my later work with Java (-XX:+PreserveFramePointer) for a different and more complete profile.
Vladimir Kirillov has developed eflame (github), a Flame Graph profiler for Erlang. Blocking calls are in blue, see the screenshot.
Trevor Norris posted instructions for creating Linux Node.js CPU flame graphs, by using perf_events and --perf-basic-prof. See the example.
Luca Canali provides many examples of Flame Graphs for Oracle databases, and explains what they mean.
Julien Charbon posted some CPU flame graphs on FreeBSD, to analyze TCP stack lock contention with short-lived connections.
Gabriel posted Profiling and optimising with Flamegraph, explaining all the steps with some really good examples.
More Flame Graph news (updated Dec 2014):

Facebook's Strobelight uses an inverted flame graph for perf analysis. See the slide in Yannick Brosseau's Using tracing at Facebook scale (PDF) talk.
The post on Profiling a Meteor app: Telescope includes flame graphs.
Adrien Mahieux added the Click to Zoom feature to the original FlameGraph software. This is really awesome!
I wrote a post on Differential Flame Graphs, showing new features I added to flame graphs for performance regression testing.
I came up with an unusual but useful use for differential flame graphs: CPI Flame Graphs, to highlight memory stall cycles.
I gave a talk at the 2014 FreeBSD Developer and Vendor Summit on Flame Graphs on FreeBSD.
I wrote a post on Node.js Flame Graphs on Linux using Linux perf_events and v8's --perf-basic-prof option.
Yunong Xiao showed how flame graphs helped solve an important production performance issue for Netflix in the post Node.js in Flames.
Oozou developed and published RubyProf::FlameGraphPrinter for ruby-prof, to generate folded stacks for flame graph generation.
Thorsten Lorenz developed a web flamegraph interface that can load new profiles, and perform a regexp search. See his example.
More Flame Graph news (updated Jun 2015):

Shawn Sterling posted about his excellent talk at Linuxfest Northwest 2014: Getting Started With Flamegraph, which includes both the slides and video (turn the volume up).
Scott Lystig Fritchie gave a great talk on profiling Erlang code including flame graphs at Erlang Factory San Francisco 2015.
I submitted tickets for a flame graph / flame chart toggle for Google Chrome (Issue 452624) and Firefox (bug 1123495). I'd love to see both implemented. The chromium ticket prompted interesting discussion and prototypes.
In my SCALE13x talk (2015), I previewed mixed-mode Java flame graphs using Linux perf_events ("perf"). This uses a JVM frame pointer patch that has become the -XX:+PreserveFramePointer option in both JDK9 (JDK-8068945) and JDK8u60 (JDK-8072465). For the first time, we can see all CPU consumers in one visualization! See slides 40 to 57 for more details, and my post about it (when I write it!).
Francesco Mazzoli post about Flame graphs for GHC time profiles with ghc-prof-flamegraph, including a tutorial for making flame graphs from the existing GHC (Haskell) profiler output.
Robin Moffatt posted about Analysing ODI performance with Flame Graphs, for understanding load plans in the Oracle Data Integrator.
Flame graphs were demoed at d3NYC (Drupal) 2015, but I haven't found slides online yet.
Cor-Paul Bezemer has been investigating flame graph differentials with his Flamegraphdiff software, which shows the difference from A to B using three flame graphs simultaneously. This was also the subject of a SANER2015 paper ($), and talk.
M. Isuru Tharanga Chrishantha Perera wrote a translator for Java Flight Recorder profiles, for making flame graphs.
Jan Stępień developed Pluggable Flame Graphs for Clojure (example).
Evan Hempel has created a python-flamegraph profiler for Python, which generates the folded stack output suitable for making into flame graphs. See the README on github for instructions.
Franck Pachot posted another example of using CPU flame graphs for identification of an Oracle database bug.
Strongloop have included a nice looking Node.js flame graphs module for their Arc Profiler.
@yoheia posted a detailed flame graph post showing Linux kernel profiling (in Japanese).
Zoltan Majo fixed JDK-8068945 for Java 9 and JDK-8072465 for Java 8 update 60 build 19 (or later, download as early access here). This adds -XX:+PreserveFramePointer, which allows Linux perf_events to sample full stacks for making flame graphs. This began with a prototype patch I developed and submitted: (A hotspot patch for stack profiling). Great to see this functionality make it into the JVM!
Min Zhou has developed PerfJ, for automating the collection of Java flame graphs, using the frame pointer patch (see previous item).
More Flame Graph news (updated Dec 2015):

Myself and Martin Spier posted about Java in Flames (PDF) for the Netflix Tech Blog, showing Java mixed-mode flame graphs using the new -XX:+PreserveFramePointer JVM option.
Jonathan Perkin used memory flame graphs for Reducing RAM usage in pkgin.
Isuru Perera postabout about Java CPU Flame Graphs with an example making use of Min's PerfJ.
Ben Sandler from Uber has posted go-torch, a flame graph profiler for go-lang programs.
Andi Kleen demonstrated Generating flame graphs with Processor Trace, a feature from modern Intel CPUs for very high frequency sampling.
I wrote about the new flame graph search feature. The matched percentage is very handy, so I don't have to mouse over many tiny frames and add them up manually.
Eben Freeman posted about Profiling Python in Production, which includes a python profiler and basic d3 flame graphs.
Will Sewell mentioned flame graphs in his Top tips and tools for optimising Haskell post, and linked to the work for GHC flame graphs by Francesco Mazzoli.
Bo Lopker created djdt-flamegraph (github) for getting a flame graph of current requests in the Django Python web framework.
NodeSource have flamegraphs in their NSolid Node.js analysis product. The graphics look very nice, and they also have treemaps and sunbursts (both of which I think are usually less effective than flame graphs, but I don't mind having the option).
Alex Ciminian has been developing a d3-flame-graphs library (github) for d3, implemented in CoffeeScript. Check out the demo, it looks really nice so far.
Martin Spier (a colleague at Netflix) has been developing d3-flame-graph, as a d3 library implemented in JavaScript. This will be integrated into our open source Vector instance analysis tool. It has zoom transitions!
I gave a Java Mixed-Mode Flame Graphs talk at JavaOne 2015, including all the latest updates.
David Calavera wrote about Docker flame graphs for Go.
Carol Nichols wrote detailed instructions for Rust Profiling with Instruments and FlameGraph on OSX: CPU/Time.
More Flame Graph news (updated Jun 2016):

Apekshit Sharma wrote a post about Saving CPU! Using Native Hadoop Libraries for CRC computation in HBase.
Mike Huang wrote a Netflx tech blog post to show how he helped with Saving 13 Million Computational Minutes per Day with Flame Graphs.
Srdjan Marinovic and Ryan Day created goprofui includes a cpu.go profiler for golang and flame graphs.
I wrote an article for ACMQ, The Flame Graph, which defines flame graphs, describes their origin, explains how to interpret them, and discusses possible future developments.
David Mark Clements has developed 0x, a new interactive flame graph profiler for Node.js processes on both Linux and OS X. See the demo.
Qt Creator 4.0.0 now includes flame graphs along with timeline and statistical views. That's how it should be done: different views of the same data you can switch between.
Antonio Pérez developed 4gl-flamegraph for processing the profiler output of Genero by 4Js (Four Js).
Ty Overby published a flamegraph profiling tool for rust.
The vprof Python package provides interactive visualizations for profiling, including flame graphs.
Alastair Butler and Kei Yoshimoto published the paper Large scale semantic representation with flame graphs (PDF), where they used them to visualize semantically annotated corpora, which has been parsed into a hierarchy.
Jonathan Newbrough developed the Gumshoe Load Investigator for Java analysis, initially for internal use in the Dell Cloud Manager, which makes good use of flame graphs.
Mahesh Dathrika published Igniting Node.js Flame, showing how they use the v8-profiler at eBPF to create Node.js flame graphs.
Maciek Lesiczka is building an ASP.NET profiler that includes flame graphs, and introduced it in netric.io - lightweight ASP.NET profiler.
Nitsan Wakart covered Java flame graphs and various Java profiling challenges in his Devoxx UK talk Extreme Profiling: Digging Into Hotspots by Nitsan Wakart (youtube).
Maciek Lesiczka has been developing netric.io - lightweight ASP.NET profiler, an ASP.NET profiler that includes flame graphs.
My article The Flame Graph was published in CACM.
More Flame Graph news (updated Dec 2016):

Rhys Hiltner from Twitch blogged about Go's march to low-latency GC, which included flame graph analysis of GC time.
Dmytro Semenov on the ebay tech blog posted Mastering the Fire, about how they can create flame graphs on production servers at any time with one click of a button.
Olivier Cano posted about Measuring Snap performance which includes flame graphs for golang.
Kay Ousterhout wrote about Generating Flame Graphs for Apache Spark using Java Flight Recorder.
Adam Perry published Rust Performance: A story featuring perf and flamegraph on Linux, which also has great examples of using perf.
Bruce Dawson published a post on ETW Flame Graphs Made Easy and the new support in Windows Performance Analyzer (WPA).
Aviem Zur from PayPal engineering wrote about Spark in Flames - Profiling Spark Applications Using Flame Graphs.
Cédric Champeau (I think) has added automatic Flame graph generation for Gradle's perf tests. Makes a lot of sense, as does auto generation for software versions and builds (given a load generator).
Bill Smith from indeed engineering posted A Funny Thing Happened on the Way to Java 8, where code cache issues are investigated with flame graphs (I've also found a code cache issue this way!).
Bert Hubert included flame graphs in his detailed analysis Optimizing optimizing: some insights that led to a 400% speedup of PowerDNS.
Luca Canali has posted more flame graphs, this time Apache Spark 2.0 Performance Improvements Investigated With Flame Graphs, and even includes some perf stat analysis.
Mahmoud Hatem posted about perf_events : Off/On/Mixed CPU flamegraph extended with oracle wait events, showing both on- and off-CPU flame graphs for an Oracle workload.
Evan Klitzke posted pyflame: Uber enginering's ptracing profiler for python, as well as the source on github. Python is tricky to profile as it's interpreted only, so basic frame pointer-based stack walking only identifies interpreter frames.
I posted about Linux 4.9's Efficient BPF-based Profiler, an important feature that will make flame graph profiling much more efficient.
Jerome Terry posted instructions for Generating Java Mixed Mode Flame Graphs summarizing all the latest steps.
Joel Fernandes posted ARMv8: Flamegraph and NMI Support.
More Flame Graph news (updated Jun 2017):

Linkedin have done some great work with flame graphs: ODP: An Infrastructure for On-Demand Service Profiling, including searching and comparison (differential) capabilities.
Alice Goldfuss posted about Making FlameGraphs with Containerized Java, figuring out how to get perf to work in a container environment.
Nitsan Wakart wrote a great post on Java Flame Graphs Introduction: Fire For Everyone!, that explains different Java profilers in depth.
Benoit Bernard posted Using Uber's Pyflame and Logs to Tackle Scaling Issues showing all steps.
I posted Where has my disk space gone? Flame graphs for file systems, with a follow up Flame Graphs vs Tree Maps vs Sunburst.
Sasha Goldshtein posted Profiling a .NET Core Application on Linux where he solves problems and discusses next challenges with profiling Windows .NET on Linux.
Steve Robinson posted Making sense out of flamegraphs (Ruby on Rails) with an introduction to stack sampling profilers.
Ross Schlaikjer posted Profiling Android apps with Flamegraphs, and has an online Android Trace file to flamegraph converter.
Claes Redestad posted Bytestacks visualizing the output of -XX:+TraceBytecodes
Lari Hotari has created jfr-report-tool to turn a Java Flight Recorder (JFR) dump into either CPU flame graphs or allocation flame graphs.
Nan Xiao posted Use perf and FlameGraph to profile program on Linux, including a sample C++ program to analyze.
Michael Malis at Heap posted How Basic Performance Analysis Saved Us Millions, with flame graph analysis of Postgres.
I posted Java Package Flame Graph, showing this new way to inspect Java CPU usage.
More Flame Graph news (updated Dec 2017):

My USENIX ATC 2017 talk was posted: Visualizing Performance with Flame Graphs (youtube) (sildes): this is my updated flame graphs talk.
I posted Coloring Flame Graphs: Code Hues, explaining how I've been improving them
Oracle added flame graphs to Oracle Developer Studio Performance Analyzer (PDF).
Mark Price wrote about Using Flame Graphs to Analyze & Determine Cassandra Performance Under Stress (Part 2 of 3), which includes Java thread pool names from jstack to group towers.
LinkedIn has enhanced flame graphs to do Common Issue Detection for CPU Profiling, and applied it to several common JVM issue. This is a good idea.
Alexey Ivanov wrote Optimizing web servers for high throughput and low latency, which is a grand tour of many Linux performance tools including flame graphs and bcc/BPF. Check it out.
Nitsan Wakart gave an excellent talk Exploring Java Perf Flamegraphs at JavaZone 2017.
Mark Price added Java Heap Allocation Flamegraphs to grav toolkit, by using the USDT object-alloc probe.
I included flame graphs in my Kernel Recipes 2017 talk: Using Linux perf at Netflix (youtube) (slides).
Miel Donkers posted The JVM on Fire – Using Flame Graphs to Analyse Performance, and compared a flame graph to JProfiler tree view.
Nudge APM appear to have included flame graphs in their profiling section of their analyzer (for Java, PHP, .NET).
Adam Sitnik developed flame graphs for Windows PerfView, which has been merged (yay).
Martin Spier added flame graphs to pprof's web UI.
Konrad Malawski posted Automatic FlameGraph generation from JMH Benchmarks using (SBT) JMH Extras (plain Java too).
Alexander Yakushev posted Profiling tool: async-profiler, showing how it can be used to generate flame graphs for Clojure apps on the JVM.
Ruth Grace Wong gave the talk FlameGraph That: Self-Service Profiling with SaltStack and Rundeck at Salt Conf 17 (thanks!).
Milian Wolff's Hotspot - the Linux perf GUI for performance analysis tool has flame graphs (I'm not sure exactly when it merged.)
More Flame Graph news (updated Dec 2018):

Jon Hadad wrote about Analyzing Cassandra Performance with Flame Graphs.
Amir Langer gave a talk (in Hebrew) about Flame Graphs and the JVM at eBay (youtube).
Datadog say they added flame graphs for Java performance but they look like flame charts. I'm not sure yet.
Brendan Ryan wrote about Profiling Go Applications with Flamegraphs using Uber's go-torch library.
Kiran posted How to use linux perf tools and save dollars - Part 2, including flame graphs of perf samples.
Vladimir Agafonkin developed Flamebearer, for lightweight responsive graphs for Node.js and v8.
Trace Compass added flame graphs as well as flame charts, the first tool to support both (yay).
Myself and my Netflix colleague Martin Spier published FlameScope, a new performance analysis tool that allows profiles to be visualized as subsecond-offset heat maps, and then time ranges to be selected and then visualized as a flame graph. It's also on github Netflix/flamescope.
Sadiq Jaffer, Richard Warburton wrote about Always-on production Flame Graphs for Java.
Alexandru Olaru posted Squeeze node.js performance with flame graphs.
Ivan Babrou posted a case study of https://blog.cloudflare.com/tracing-system-cpu-on-debian-stretch/, including flame graphs (and bcc/eBPF tools).
Marcus Hirt developed jmc-flame-view and posted instructions for getting it going: this adds flame graphs to Java Mission Control.
Jamie Wong has developed SpeedScope - Interactive Flamegraph Explorer, which supports "time order" (flame charts), "left heavy", which are flame graphs with an additional sort so the largest frames move left, and "sandwich" ("middle-out" merge). Great to see all three types in one GUI.
More Flame Graph news (updated Oct 2019):

Nina Li posted Visualizing OLAP Requests on SAP HANA System with Concurrency Flame Graph using SAP HANA Dump Analyzer.
The AMD uProf performance analysis tool now includes flame graphs.
The YourKit Java profiler has added flame graphs for its 2019.8 release (currently EAP).
The IntelliJ IDE now has flame graphs to visualize stacks from Java Flight Recorder or Async profiler.
Michael Hunger wrote a post Firing on All Engines: Flame graphs for Java Programs for the 97 Things from O'Reilly Media blog.
Agustin Gallego wrote the post Profiling Software Using perf and Flame Graphs showing MySQL server as a target (the same use case that led me to first create flame graphs.)
More Flame Graph news (updated Oct 2020):

Amazon launched the AWS CodeGuru profiler that includes flame graphs. Also see the flame graph examples in Optimizing application performance with Amazon CodeGuru Profiler.
Google added flame graphs to Google Cloud Profiler. By default these use a largest-frame-left sort (I'd rather alphabetic so that towers don't switch positions when comparing two profiles from the same system). There are flame graph examples in How Mercari reduced request latency by 15% with Cloud Profiler.
There was more news, I'm getting behind adding links here. Will catch up.
Thanks to everyone who has written about flame graphs, developed them further, and shared their results! I'll update this page from time to time with more news.
