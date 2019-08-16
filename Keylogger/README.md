# Advanced Keylogger

A personal project where I attempt to create an advanced Keylogger in C++.

## Disclaimer:
**This project is for learning and educational purposes only. Please do not use this program for illegal purposes!**

### Goals for project:

* Learn about ethical hacking, computer security and cryptography
* Find a way to implement threading (generally for more practice)
* Aim to create a program that is undetectable by anti-virus software
* Implement encryption to all key logs
* Encrypted key logs will be written to a file and sent to user's email
* Include method of decrypting file sent to user's email

### Encoding via Base64:

~~~
/*	Base64 Algorithm 

	Base64 Chart:

	 ------------------------------------------------------
	|  A  |  B  |  C  |  D  | ... |  8  |  9  |  +  |  /  |
	 ------------------------------------------------------
	|  0  |  1  |  2  |  3  | ... | 60  | 61  | 62  | 63  |
	 ------------------------------------------------------

	 Chart includes (64 characters): 
	 ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/

	 For an example of how this encoding process works we will use the 
	 word "Code" which is "Q29kZQ==" in its Base64 encoded form.

	 1) When you take the letters of "Code" and translate each one into
	 ASCII values we get:
		C = 067
		o = 111
		d = 100
		e = 101

	2) Translated into 8-bit binary these ASCII values are:
		067 = 01000011
		111 = 01101111
		100 = 01100100
		101 = 01100101

	3) The binary values grouped together (8x4 = 32 bits):
		01000011 + 01101111 + 01100100 + 01100101 =
		01000011011011110110010001100101

	4) Then this 32-bit number is grouped into groups of 6 
	(because 2^6 = 64 and this allows us to group them into our chart)

		-------------------------------------------
		|010000|110110|111101|100100|011001|010000|  <-- add 4 zeros to last group
		-------------------------------------------
		|            Convert to Decimal           |	
		-------------------------------------------
		|  16  |  54  |  61  |  36  |  25  |  16  |
		-------------------------------------------
		|     Convert Decimal to Base64 values    |
		-------------------------------------------
		|  Q   |  2   |  9   |  k   |  Z   |  Q   |
		-------------------------------------------

	5) This gives us "Q29kZQ", but because we had a group of less than
	   6 bits, we have to add "padding" to this value, which is a "="
	   symbol in Base64. If our number modulus 3 (32 % 3 = 2) is equal 
	   to 2, we add two equal signs to our value as padding, resulting 
	   in the final encoded value of: "Q29kZQ=="

	   If the bit number divided by 3 resulted in a remainder of 1
	   then we would only add one equal sign for padding. Here are the 
	   respective formulas:
	   
	   n % 3 = 1(bit) --> then we add "=" at the end for padding
	   n % 3 = 2(bits) --> then we add "==" at the end for padding
	   
 */
 ~~~

### Log Files:

Encrypted log files will be saved on the local computer before being sent via email.<br/>

![logfiles](https://user-images.githubusercontent.com/24645219/44196435-c6a38300-a0f0-11e8-99f8-e7f13a748cd8.jpg)

### Emailed Log Files:

A copy of the log file will be sent via email every time interval as long as the program is running.<br/>
A decrypted version of the log file will also appear inside the body of the email.<br/>

![email](https://user-images.githubusercontent.com/24645219/44196434-c6a38300-a0f0-11e8-9f81-ea717461df46.jpg)

### Decryption Tool:

The file will be decrypted inside the email, but if the user wishes to decrypt the files on their
local machine, I have created a separate tool for decryption for this specific keylogger 
which can be found **[Here](https://github.com/Falcon-Punch/KeyLog-Decryptor)**.

