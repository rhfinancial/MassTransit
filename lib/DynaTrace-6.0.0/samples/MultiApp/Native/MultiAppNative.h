#ifndef __MULTIAPPNATIVE_H__
#define __MULTIAPPNATIVE_H__

/** 
 *	Main application class
 */
class MultiAppNative {
public:
	MultiAppNative();
	virtual ~MultiAppNative();
	void run();

	/** Helper function for calculation custom tag from payload */
	static void getCustomTagFromData(unsigned char* buffer, long long a, long long b);
};

// define size for our custom tag
#define CTAG_SIZE  8

#endif // __MULTIAPPNATIVE_H__
