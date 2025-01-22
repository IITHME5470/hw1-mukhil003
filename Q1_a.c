
#include <stdio.h>
#include <stdlib.h>

void write_array_to_file(int n,double **A,int format_flag)
{
	char filename[50];
	int i,j;
	snprintf(filename,sizeof(filename),"array_%06d_%s.out",n,format_flag == 0 ? "asc":"bin");

	FILE *file = fopen(filename,format_flag == 0 ? "w":"wb");
	if(file == NULL)
	{
		printf("ERROR!");
		return;
	}
	if(format_flag == 0)
	{
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
				fprintf(file,"%.15e ",A[i][j]);
			fprintf(file,"\n");
		}
	}
	else
	{
		for(i=0;i<n;i++)
			fwrite(A[i],sizeof(double),n,file);
	}
	fclose(file);
	printf("Array written to file '%s'.\n",filename);
}

int main()
{
	
  int n;
  FILE *fptr = fopen("input.in","r");
	if(fptr == NULL)
	{
		printf("ERROR! Unable to open the file.\n");
		return(1);
	}

	if(fscanf(fptr,"%d",&n) != 1)
	{
		printf("ERROR!\n");
		fclose(fptr);
		return(1);
	}
	fclose(fptr); 
 
 
	int i,j,k;
	double **A = (double **)malloc(n*sizeof(double *));
	if(A == NULL)
	{
		printf("Memory allocation failed!\n");
		return(1);
	}
	for(i=0;i<n;i++)
	{
		A[i] = (double *)malloc(n*sizeof(double));
		if(A[i] == NULL)
		{
			printf("Memory allocation failed!\n");
			for(k=0;k<i;k++)
				free(A[k]);
			free(A);
			return(1);
		}
	}

	for(i=0;i<n;i++)
	{
		for(j=0;j<n;j++)
			A[i][j] = i+j;
	}

	write_array_to_file(n,A,0);
	
	write_array_to_file(n,A,1);
	
	for(i=0;i<n;i++)
		free(A[i]);
	free(A);

	return(0);

}
