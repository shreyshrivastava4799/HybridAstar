#ifndef VORONOI
#define VORONOI


#include"opencv2/highgui/highgui.hpp"
#include"opencv2/core/core.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include<iostream>

#include"bfs.hpp"
#include"draw_bfs.hpp"
#include"edge_cost.hpp"

using namespace cv;
using namespace std;
#include<queue>


Mat calculate_voronoi_values(Mat obs_dist,Mat voronoi_dist){
	Mat outpu = obs_dist.clone();
	for(int i=0;i<obs_dist.rows;i++){
		for(int j=0;j<obs_dist.cols;j++){
			if(obs_dist.at<uchar>(i,j)!=255){
				if(obs_dist.at<uchar>(i,j)>max_obs_dist){
					outpu.at<uchar>(i,j) = is_voronoi*254;
					continue;
				}
				outpu.at<uchar>(i,j) = is_voronoi*(int)(255*(1-((1.0*alpha/(1.0*alpha+1.0*obs_dist.at<uchar>(i,j)))*(1.0*voronoi_dist.at<uchar>(i,j)/(1.0*voronoi_dist.at<uchar>(i,j)+1.0*obs_dist.at<uchar>(i,j)))*((1.0*max_obs_dist-1.0*obs_dist.at<uchar>(i,j))/(1.0*max_obs_dist))*((1.0*max_obs_dist-1.0*obs_dist.at<uchar>(i,j))/(1.0*max_obs_dist)))));
			}
			else outpu.at<uchar>(i,j)=0;
			if(!is_voronoi) outpu.at<uchar>(i,j) = 255;

		}
	}
	/*for(int i=0;i<outpu.rows;i++) for(int j=0;j<outpu.cols;j++) outpu.at<uchar>(i,j) = 254;
	for(int i=0*outpu.rows;i<0.5*outpu.rows;i++) for(int j=0*outpu.cols;j<0.5*outpu.cols;j++) outpu.at<uchar>(i,j) =0 ;
	*/
	return outpu;
}

void voronoi(Mat input)
{
//Use this section to get costmap from ROS into input grayscale image
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Mat input_border(input.rows,input.cols,CV_8UC1,Scalar(0));
	int i,j;
	//cout <<"dghftht" <<endl;
	//To binarise the image
	for(i=0;i<input.rows;i++)
	{
		for(j=0;j<input.cols;j++)
		{
			if(input.at<uchar>(i,j)>30)
				input.at<uchar>(i,j)=255;
			else
				input.at<uchar>(i,j)=0;
		}
	}
	//cout << 2 <<endl;

//To invert the image(comment this section if it is not required to be inverted i.e. if the obstacles are already black)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	Mat input_inverted=input.clone();
	for(i=0;i<input_inverted.rows;i++)
	{
		for(j=0;j<input_inverted.cols;j++)
		{
			if(input.at<uchar>(i,j)==255)
				input.at<uchar>(i,j)=0;
			else
				input.at<uchar>(i,j)=255;
		}
	}
	*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	// To generate input image which consists of white pixels representing borders of the obstacle objects as input_border the original image
	 for(i=1;i<input.rows-1;i++)
	{
		for(j=1;j<input.cols-1;j++)
		{	
			
			if((int)input.at<uchar>(i,j)==0)
			{
				if((int)input.at<uchar>(i+1,j)==255||(int)input.at<uchar>(i+1,j+1)==255||(int)input.at<uchar>(i-1,j)==255||(int)input.at<uchar>(i,j-1)==255||(int)input.at<uchar>(i,j+1)==255||(int)input.at<uchar>(i-1,j-1)==255||(int)input.at<uchar>(i+1,j-1)==255||(int)input.at<uchar>(i-1,j+1)==255)
				{	
					input_border.at<uchar>(i,j)=255;
				}
			}
		}
	}
	//cout << 3 <<endl;
	bool flag1=true;
	for(i=0;i<input.rows;i++)
	{
		for(j=0;j<input.cols;j++)
		{
			if(input.at<uchar>(i,j)==255)
			flag1=false;
		}
	}
	if(flag1)
	{
		final=Mat(input.rows,input.cols,CV_8UC1,Scalar(255));
		obs_dist_global=Mat(input.rows,input.cols,CV_8UC1,Scalar(255));
	//	cout<<"sdjbksd"<<endl;
		return;
	}
	Mat output_colored;
	Mat pushed;
	common = Mat(input.rows,input.cols,CV_8UC1,Scalar(0));
	main_bfs(input_border);
	//cout << "fd"<<endl;

	Mat output_regions(input.rows,input.cols,CV_8UC1,Scalar(0));
	imshow("common",common);
	output_regions=find_obstacle_dist(input);
	//cout << "wtf 1" <<endl;

	find_edge_cost(voronoi_edges);
	//cout << "wtf 2" <<endl;
	obs_dist_global=cost_image.clone();
	namedWindow("Final",WINDOW_NORMAL);  //To show the results 

//Comment these lines if you don't want the intermediate steps to be printed
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	/*imshow("Input",input);
	imshow("Input with Borders",input_border);
	imshow("Output",output_regions);
	imshow("obstacle_cost_image",obs_dist_global);
	//for(int i=0;i<input.rows;i++) for(int j=0;j<input.cols;j++) voronoi_cost_image.at<uchar>(i,j) = 255; 
	imshow("voronoi_cost_image",voronoi_cost_image);
	imshow("voronoi_edges_image",voronoi_edges);
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	createTrackbar("Alpha","Final",&alpha,1000);
	createTrackbar("Max Obsacle Distance","Final",&max_obs_dist,1000);
	createTrackbar("USE_VORoNOI","Final",&is_voronoi,1);
	
	//while(1){
		final = calculate_voronoi_values(cost_image,voronoi_cost_image);
		imshow("Final",final);
		//cout<<"fhvk"<<endl;
		waitKey(10);
	//}
	//waitKey(0);
	
}

#endif