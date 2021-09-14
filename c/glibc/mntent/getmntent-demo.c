#include <stdio.h>
#include <stdlib.h>
#include <mntent.h>

int main(void)
{
  struct mntent *ent;
  FILE *aFile;

  ///proc/self/mountinfo

  ///proc/mounts 或者是 /etc/mtab
  aFile = setmntent("/proc/mounts", "r"); 
  if (aFile == NULL) {
    perror("setmntent");
    exit(1);
  }
  while (NULL != (ent = getmntent(aFile))) {
    printf("mnt_fsname:%s mnt_dir: %s mnt_type:%s mnt_opts: %s\n", 
                ent->mnt_fsname, ent->mnt_dir, 
                ent->mnt_type, ent->mnt_opts);
//    break;
  }
  endmntent(aFile);
}

