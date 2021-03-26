$(document).ready(function () {	
	var questionNumber=0;
	var questionBank=new Array();
	var stage="#game1";
	var stage2=new Object;
	var questionLock=false;
	var numberOfQuestions;
	var score=0;

	questionBank=new Array;
	questionBank = new Array;	//["Question", "Right Answer", "Wrong Answer", "Wrong Answer"],
    questionBank = [["What are the different types of memory sections?", "Text, Data, BSS, Heap, Stack", "Text, Owners, Size, Heap, Stack", "Text, Mode, BSS, Heap, Reference Count"],
	["What is stored in text?", "Instructions that the program runs", "Memory returned when calling malloc/new", "Size of file in bites"],
	["What is stored in data?", "Initialized global variables", "Uninitialized global variables", "Instructions that the program runs"],
	["What is stored in BSS?", "Uninitialized global variables", "Initialized global variables", "Local variables and return addresses"],
	["What is stored in the heap?", "Memory returned when calling malloc/new. It grows upward", "Local variables and return addresses. It grows downward", "Uninitialized global variables"],
	["What is stored in the stack?", "Local variables and return addresses. It grows downward", "Memory returned when calling malloc/new. It grows downward", "Initialized global variables"],
	["What are dynamic libraries?", "Libraries shared with other processes", "Libraries that are copied into the executable", "Libraries where you can read executables"],
	["What are some examples of executable formats?", "ELF, COFF", "DOG, EXE", "LINK, JPG"],
	["What are the different steps to build a program?", "Editor, C preprocessor, Compiler, Optimizer, Assembler, Linker, Loader", "Editor, Compiler, Optimizer, C preprocessor, Linker, Assembler, Loader", "Editor, Assembler, C preprocessor, Linker, Compiler, Optimizer, Loader"],
	["What is a memory leak?", "Objects in memory that are no longer in use by the program but that are not freed", "An object that is still in use by the program is freed", "Freeing an object that is already free"],
	["What is a premature free?", "An object that is still in use by the program is freed", "Freeing an object that is already free", "Allocating less memory than the memory that is used"],
	["What is a double free?", "Freeing an object that is already free", "Objects in memory that are no longer in use by the program but that are not freed", "An object that is still in use by the program is freed"],
	["What is a wild free?", "Freeing a pointer in memory that was not returned by malloc", "Freeing an object that is already free", "An object that is still in use by the program is freed"],
	["What is a thread?", "The path of execution for a program", "A string of executables", "An application that runs in the background"],
	["What are the advantages of threads?", "Fast thread creation, fast context switch, faster communication", "Better syncing, more robust, faster communication", "Fast thread creation, less robust, faster communication"],
	["What are the disadvantages of threads?", "Less robust, more sync problems", "slower communication, slow thread creation", "loss of data, slower context switching"],
	["What are the two mutex lock implementations?", "Disable interrupts, spin locks", "Switch locks, spin locks", "Regular locks, smart locks"],
	["What is deadlock?", "One or more threads block forever and wait forever", "Creating too many threads", "Running out of memory and breaking the threads"],
	["What is starvation?", "When a thread may need to wait for a long time for a resource", "A thread waits forever", "Creating too many threads"],
	["What is the internet a collection of?", "Networks, routers, and hosts", "Interfaces, ethernet, routers", "Applications, transport, layering"],
	["What are the two parts of an IP address?", "Network ID and host ID", "Router ID and network ID", "Interface ID and network ID"],
	["What is a subnet mask?", "An interface parameter that tells the number of bits for the network number", "The hidden IP address", "The route to particular destinations"],
	["What are the 4 components of a Dynamic Host Configuration Protocol(DHCP)?", "Local IP address, subnet mask, default router, DNS server", "Network ID, subnet mask, global internet, DNS server", "Local IP address, secret mask, global host, DNS server"],
	["What are the two transport protocols?", "UDP and TCP", "ELF and JPG", "IMG and TTPS"],
	["What is Structured Query Language(SQL)?", "The standard language used to manipulate databases", "The standard language for low level programming", "The standard language for programming websites"],
	["What are the three different SQL databases?", "MySQL, Orace, and Microsoft SQL", "SQL, MSQL, and Super SQL", "MySQL, Enterprise, and SQL"],
	["How many components are in Joel's test?", "12", "7", "15"],
	["What is extreme programming?", "A practical methodology for software development", "A methodology best used for large groups with poor communication", "A way for several developers to wokr on the same code"],
	["What are some different source control programs?", "CVS, SVN, Mercurial, Git", "UDP, MSN, CVS, Git", "ELF, TCP, Mercurial, Git"],
	["Why are program tests important?", "They are the most practical way to improve the quality of a program", "They are a way to get a grade", "Tests written to reproduce a bug and also verify the bug has been fixed"],
	["What are regression tests?", "Tests written to reproduce a bug and also verify the bug has been fixed", "The most practical way to improve the quality of a program", "A group of tests that specify a specific class"],
	["What are acceptance tests?", "Tests that evaluate the software before a release to tell if a product is ready or not", "Tests written to reproduce a bug and also verify the bug has been fixed", "A group of tests that specify a specific class"],
	["When should you refactor?", "Duplicate code, large class, inconsistent naming, dead code, too general", "Duplicate code, small class, bad naming, hard to read code, bad code", "Incomplete code, errors, inconsistent naming, dead code, wrong language"],
	["What does ARP mean?", "Address Resolution Protocol", "Additional Resource Protocol", "Ancient Reading Protocol"],
	["What does DNS mean?", "Domain Name Services", "Dynamic Name Sequencer", "Domain Name Sequencer"],
	["What does DHCP mean?", "Dynamic Host Configuration Protocol", "Domain Host Configuration Protocol", "Digital Host Configuration Protocol"],
	["What is a source control system?", "A central history to keep track of changes and updates", "A way to make a program", "A system to update all programs on a computer"],
	["What is external fragmentation?", "Small non-contiguous block allocation such that they cannot satisfy the request", "Waste of memory due to the allocator returning a larger block than the memory requested", "A way to allow for constant time coalescing"],
	["What is internal fragmentation?", "Waste of memory due to the allocator returning a larger block than the memory requested", "Small non-contiguous block allocation such that they cannot satisfy the request", "A way to allow for constant time coalescing"],
	["What are the yellow pages?", "User information and passwords stored in a server", "A phone book", "Where everything is stored on a computer"],
	["What is an Inode?", "A file or directory", "A program", "A web browser"],
	["What does UDP mean?", "User Datagram Protocol", "Usage Diagram Protocol", "User Diagnostic Protocol"],
	["What does TCP mean?", "Transmission Control Protocol", "Triangular Category Protocol", "Topological Control Protocol"],
	["What does NAT mean?", "Network Address Translation", "Neutral Address Transmission", "National Address Transport"],
	["What are system calls?", "A way user programs request services from the OS", "A device interrupt, such as moving the mouse", "An event that requires immediate attention"],
	["What is an interrupt?", "An event that requires immediate attention", "A way user programs request services from the OS", "Talking in class when you should not be"],
	["Is polling synchronous or asynchronous?", "Synchronous", "Asynchronous", "Neither"],
	["Is an interrupt synchronous or asynchronous?", "Asynchronous", "Synchronous", "Neither"],
	["What is the kernal mode?", "A mode where the CPU can execute any instruction and modify and location in memory", "A mode where the CPU has limited access to instructions and can modify some locations in memory", "A mode where the CPU has no access and cannot modift anything"],
	["What is the user mode?", "A mode where the CPU has limited access to instructions and can modify some locations in memory", "A mode where the CPU can execute any instruction and modify and location in memory", "A mode where the CPU has no access and cannot modift anything"]]

	numberOfQuestions=questionBank.length;
	randomizeQuestions();
	displayQuestion();

	function randomizeQuestions(){
		for(i=0; i<100;i++){
			var rnd1=Math.floor(Math.random()*questionBank.length);
			var rnd2=Math.floor(Math.random()*questionBank.length);
			var temp=questionBank[rnd1];
			questionBank[rnd1]=questionBank[rnd2];
			questionBank[rnd2]=temp;
		}
	}

	function displayQuestion(){
		var rnd=Math.random()*3;
		rnd=Math.ceil(rnd);
		var q1;
		var q2;
		var q3;

		if(rnd==1){q1=questionBank[questionNumber][1];q2=questionBank[questionNumber][2];q3=questionBank[questionNumber][3];}
		if(rnd==2){q2=questionBank[questionNumber][1];q3=questionBank[questionNumber][2];q1=questionBank[questionNumber][3];}
		if(rnd==3){q3=questionBank[questionNumber][1];q1=questionBank[questionNumber][2];q2=questionBank[questionNumber][3];}

		$(stage).append('<div class="questionText">'+questionBank[questionNumber][0]+'</div><div id="1" class="option">'+q1+'</div><div id="2" class="option">'+q2+'</div><div id="3" class="option">'+q3+'</div>');

		$('.option').click(function(){
		if(questionLock==false){
			questionLock=true;	
			//correct answer
			if(this.id==rnd){
				$(stage).append('<div class="right">CORRECT</div>');
				score++;
			}
			//wrong answer	
			if(this.id!=rnd){
				$(stage).append('<div class="wrong">WRONG</div>');
			}
			setTimeout(function(){changeQuestion()},1000);
		}})
	}//display question

	function changeQuestion(){
		questionNumber++;
	
		if(stage=="#game1"){
			stage2="#game1";stage="#game2";
		}else{
			stage2="#game2";stage="#game1";
		}
	
		if(questionNumber<numberOfQuestions){
			displayQuestion();
		}else{
			displayFinalSlide();
		}
	
		$(stage2).animate({"right": "+=800px"},"slow", function() {$(stage2).css('right','-800px');$(stage2).empty();});
		$(stage).animate({"right": "+=800px"},"slow", function() {questionLock=false;});
	}//change question
	
	function displayFinalSlide(){
		var percent=Math.floor((score/numberOfQuestions)*100);
		$(stage).append('<div class="questionText">Good Luck on Exams!<br><br>Total questions: '+numberOfQuestions+'<br>Correct answers: '+score+'<br>Percentage: '+percent+'%</div>');
		
	}//display final slide
});//doc ready