#include<bits/stdc++.h>
using namespace std;

#define DISKSIZE 500*1024*1024
#define BLOCKSIZE 20*1024

int main()
{
	unordered_map<string,vector<long long int>> files;
	unordered_map<string,FILE*> disks;
	unordered_map<string,int> descriptor;
	int no_of_inodes = BLOCKSIZE/sizeof(int);
	vector<int> inode(no_of_inodes);
	unordered_map<int,pair<string,int>> filedata;
	inode[0]=1;
	int i,j;
	for(i=1;i<no_of_inodes;i++)
	inode[i]=0;
	int choice;
	int fdes=0;
	while(1)
	{
		cout<<"1 - Create disk \n2 - Mount disk \n3 - Exit"<<endl;
		cout<<"Enter your choice: ";
		cin>>choice;
		if(choice == 1)
		{
			cout<<"Enter disk name: ";
			string dname;
			cin>>dname;
			disks[dname]=NULL;
			cout<<"Disk "<<dname<<" is successfully created"<<endl;
		}
		else if(choice == 2)
		{
			string diskname;
			cout<<"Enter disk name: ";
			cin>>diskname;
			if(disks.find(diskname) == disks.end())
			{
				cout<<"Disk "<<diskname<<" not created"<<endl;
				continue;
			}
			FILE* fp=fopen(diskname.c_str(),"wb+");
			disks[diskname]=fp;
			while(1)
			{
				cout<<"1 - Create file \n2 - Open file \n3 - Read file \n4 - Write file \n5 - Append file \n6 - Close file \n7 - Delete file \n8 - List files \n9 - List opened files \n10 - Unmount"<<endl;
				int ch;
				cin>>ch;
				if(ch == 1)
				{
					cout<<"Enter file name: ";
					string filename;
					cin>>filename;
					
					if(files.find(filename)!=files.end())
					{
						cout<<"File "<<filename<<" exists"<<endl;
						continue;
					}
					
					for(i=0;i<inode.size();i++)
					{
						if(inode[i]==0)
						break;
					}
					
					long long int starting_point = i*BLOCKSIZE;
					inode[i]=1;
					files[filename] = {starting_point,i,0};
					
					descriptor[filename]=fdes++;
				}
				else if(ch == 2)
				{
					string filename;
					cout<<"Enter filename: ";
					cin>>filename;
					
					if(files.find(filename)==files.end())
					{
						cout<<"File "<<filename<<" does not exist"<<endl;
						continue;
					}
					
					cout<<"Enter mode in which you want to open the file \n0:read mode \n1:write mode \n2:append mode"<<endl;
					int mode;
					cin>>mode;
					
					filedata[descriptor[filename]] = {filename,mode};
				}
				else if(ch == 3)
				{
					int fd;
					cout<<"Enter file descriptor to read: ";
					cin>>fd;
					pair<string,int> p=filedata[fd];
					string filename=p.first;
					int mode=p.second;
					vector<long long int> v=files[filename];
					long long int sp=v[0],inode_num=v[1],filesize=v[2];
					cout<<sp<<" "<<inode_num<<" "<<filesize<<endl;
					if(mode!=0)
					{
						cout<<"Filename "<<filename<<" not opened in read mode"<<endl;
						continue;
					}				
					
					if(!filesize)
					{
						cout<<"File "<<filename<<" is empty"<<endl;
						continue;
					}
					char readbuf[20*1024];
					fseek(fp,sp,SEEK_SET);
					fread(readbuf,1,filesize,fp);
					cout<<readbuf<<endl;
					
					cout<<endl<<"File "<<filename<<" read successfully"<<endl; 
				}
				else if(ch == 4)
				{
					int fd;
					cout<<"Enter file descriptor to write: ";
					cin>>fd;
					pair<string,int> p=filedata[fd];
					string filename=p.first;
					int mode=p.second;
					vector<long long int> v=files[filename];
					long long int sp=v[0],inode_num=v[1],filesize=v[2];
					
					if(mode!=1)
					{
						cout<<"Filename "<<filename<<" not opened in write mode"<<endl;
						continue;
					}				
					string writebuf;
					getline(cin,writebuf,'$');
					fseek(fp,sp,SEEK_SET);
					//writebuf.pop_back();
					cout<<writebuf<<endl;
					long long int len=writebuf.size();
					fwrite(writebuf.c_str(),1,len,fp);
					files[filename][2]=len;
					
					cout<<files[filename][0]<<" "<<files[filename][1]<<" "<<files[filename][2]<<endl;
					
					cout<<endl<<"File "<<filename<<" written successfully"<<endl; 
				}
				else if(ch == 5)
				{
					int fd;
					cout<<"Enter file descriptor to append: ";
					cin>>fd;
					pair<string,int> p=filedata[fd];
					string filename=p.first;
					int mode=p.second;
					vector<long long int> v=files[filename];
					long long int sp=v[0],inode_num=v[1],filesize=v[2];
					
					if(mode!=2)
					{
						cout<<"Filename "<<filename<<" not opened in append mode"<<endl;
						continue;
					}				
					
					string writebuf;
					getline(cin,writebuf,'$');
					fseek(fp,sp+filesize,SEEK_SET);
					writebuf.pop_back();
					long long int len=writebuf.size();
					fwrite(writebuf.c_str(),1,len,fp);
					files[filename][2]=filesize+len;
					
					
					cout<<endl<<"File "<<filename<<" appended successfully"<<endl;
				}
				else if(ch == 6)
				{
					int fd;
					cout<<"Enter file descriptor to close: ";
					cin>>fd;
					filedata.erase(fd);
				}
				else if(ch == 7)
				{
					
				}
				else if(ch == 8)
				{
					
				}
				else if(ch == 9)
				{
					
				}
				else if(ch == 10)
				break;
				else
				{
					cout<<"Wrong input"<<endl;
				}
			}
			
		}
		else if(choice == 3)
		{
			break;
		}
	}
}
