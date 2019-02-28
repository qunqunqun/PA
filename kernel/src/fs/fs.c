#include "common.h"
#include "string.h"

typedef struct {
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

typedef struct {
	bool used;
	uint32_t index;
	uint32_t offset;
} Fstate;

enum {SEEK_SET, SEEK_CUR, SEEK_END};

static const file_info file_table[] = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440},
	{"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168},
	{"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896},
	{"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164},
	{"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609},
	{"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401},
	{"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689},
	{"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709},
	{"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225},
	{"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915},
	{"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993},
	{"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411},
	{"word.dat", 5650, 28155717},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

Fstate files[NR_FILES + 3];

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

/* Please implement a simple file system here */

int fs_open(const char *pathname, int flags) {
	int i = 0;
	for(i = 0; i < NR_FILES; i++)//find the file
	{
		if(strcmp(file_table[i].name,pathname) == 0 )
		{
			break;
		}
	}
	assert( i < NR_FILES );
	int num = i + 3;//xulie
	files[num].used = true;
	files[num].index = i;
	files[num].offset = 0;
	return num;
}

size_t fs_read(int fd, void *buf, size_t len) {
	assert(fd > 2);
	assert(fd < NR_FILES + 3);
	if(files[fd].used == false)
	{
		Log("fd:%x",fd);
	}
	assert(files[fd].used == true);
	assert(files[fd].offset >= 0);
	uint32_t index = files[fd].index;
	uint32_t filesz = file_table[index].size;
	uint32_t disk_offset = file_table[index].disk_offset;
	//OVERSIZE
	if(files[fd].offset + len >= filesz)
	{
		len = filesz - files[fd].offset;
	}
	ide_read( buf, disk_offset + files[fd].offset, len);
	files[fd].offset += len;//du qu
	return len;
}

size_t fs_write(int fd, void *buf, size_t len) {
	if(fd <= 2){
		#ifdef HAS_DEVICE_SERIAL
		int i;
		extern void serial_printc(char);
		for(i = 0; i < len; i ++) {
			serial_printc( ((char *)buf)[i] );
		}
	#else
		asm volatile(".byte 0x82" : "=a" (len) : "a"(4), "b"(fd), "c"(buf), "d"(len));
	#endif
	}
	else
	{
		assert(fd < NR_FILES + 3);
		assert(files[fd].used == true);
		assert(files[fd].offset >= 0);
		uint32_t index = files[fd].index;
		uint32_t filesz = file_table[index].size;
		uint32_t disk_offset = file_table[index].disk_offset;
		if(files[fd].offset + len >= filesz)
		{
			len = filesz - files[fd].offset;
		}
		ide_write( buf, disk_offset + files[fd].offset, len);
	}
	return len;
}

off_t fs_lseek(int fd, off_t offset, int whence) {
	//panic("Please implement fs_lseek at fs.c");
	assert(fd >= 3);
	assert(fd < NR_FILES + 3);
	uint32_t index = files[fd].index;
	uint32_t filesz = file_table[index].size;//wen jian da xiao

	switch(whence)
	{
		case SEEK_SET:
			{
				//assert( offset>=0 && offset <= filesz);
				files[fd].offset = offset;
				return offset;
			};
		case SEEK_CUR:
			{
				//assert( offset >=0 && files[fd].offset + offset <= filesz);
				files[fd].offset = files[fd].offset + offset;
				return files[fd].offset;
			};
		case SEEK_END:
			{
				files[fd].offset = filesz + offset;
				return files[fd].offset;		
			};
		default: 
			{
				assert(0);
			 	return -1;
			};
	}
}

int fs_close(int fd) {
	//panic("Please implement fs_close at fs.c");
	assert(fd >= 3);
	assert(fd < NR_FILES + 3);
	//assert(files[fd].used == true);
	files[fd].used = false;
	return 0;
}
