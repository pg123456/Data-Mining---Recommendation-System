/************************************************************
* Name: Patrick Guo
* ID: 11378369
************************************************************/
#include "MovieDatabase.h"
#include <ostream>

MovieDatabase db("movies.csv", "ratings.csv");

void SolveProblemB(std::ofstream& fout, std::vector<MoviePairAndCosineSimilarity> &movie_pairs_and_cosine_similarities)
{
	std::cout << "Logging answers for Problem B..." << std::endl;

	for (int i = 0; i < movie_pairs_and_cosine_similarities.size(); i++)
	{
		MoviePairAndCosineSimilarity current_data = movie_pairs_and_cosine_similarities[i];
		fout << "Movie ID Pairs: " << current_data.Movie1ID << " and " << current_data.Movie2ID << " have a similarity score of: " << current_data.Cosine_Similarity << std::endl;
	}
}

void SolveProblemC(std::ofstream& fout, std::map<int, NeighborhoodSetOf5> &movieIDs_to_neighbor_sets_of_5)
{
	std::cout << "Logging answers for Problem C..." << std::endl;

	for (std::map<int, NeighborhoodSetOf5>::iterator i = movieIDs_to_neighbor_sets_of_5.begin(); i != movieIDs_to_neighbor_sets_of_5.end(); i++)
	{
		NeighborhoodSetOf5 current_data = i->second;
		fout << db.GetMovieName(current_data.Reference_MovieID) << "'s movie neighbors IDs:" << current_data.Similar_MovieIDs[0] << ", " << current_data.Similar_MovieIDs[1] << ", " << current_data.Similar_MovieIDs[2] << ", " << current_data.Similar_MovieIDs[3] << ", " << current_data.Similar_MovieIDs[4] << std::endl;
	}
}

void SolveProblemD(std::ofstream& fout, std::map<int, EstimatedUserRatings> &userIDs_to_estimated_user_ratings)
{
	std::cout << "Logging answers for Problem D..." << std::endl;

	for (std::map<int, EstimatedUserRatings>::iterator i = userIDs_to_estimated_user_ratings.begin(); i != userIDs_to_estimated_user_ratings.end(); i++)
	{
		EstimatedUserRatings current_data = i->second;
		fout << "User " << current_data.UserID << " has estimated ratings of(movie ID and rating):" << std::endl;
		for (int i = 0; i < current_data.MovieIDs.size(); i++)
		{
			fout << current_data.MovieIDs[i] << ", " << current_data.Estimated_Ratings[i] << std::endl;
		}
	}
}

void SolveProblemE(std::ofstream& fout, std::map<int, GeneralHelper::ArrayOf5> &userIDs_to_top_5_recommended_movieIDs)
{
	std::cout << "Logging answers for Problem E..." << std::endl;

	for (std::map<int, GeneralHelper::ArrayOf5>::iterator i = userIDs_to_top_5_recommended_movieIDs.begin(); i != userIDs_to_top_5_recommended_movieIDs.end(); i++)
	{
		int current_user = i->first;
		GeneralHelper::ArrayOf5 current_data = i->second;
		fout << "User " << current_user << " has been recommended the movie IDs: " << current_data.Values[0] << ", " << current_data.Values[1] << ", " << current_data.Values[2] << ", " << current_data.Values[3] << ", " << current_data.Values[4] << std::endl;
	}
}

int main()
{
	std::vector<MoviePairAndCosineSimilarity> movie_pairs_and_cosine_similarities;
	std::map<int, NeighborhoodSetOf5> movieIDs_to_neighbor_sets_of_5;
	std::map<int, EstimatedUserRatings> userIDs_to_estimated_user_ratings;
	std::map<int, GeneralHelper::ArrayOf5> userIDs_to_top_5_recommended_movieIDs;
	db.GetStatistics(movie_pairs_and_cosine_similarities, movieIDs_to_neighbor_sets_of_5, userIDs_to_estimated_user_ratings, userIDs_to_top_5_recommended_movieIDs);

	std::ofstream fout;
	fout.open("output.txt");
	SolveProblemB(fout, movie_pairs_and_cosine_similarities);
	SolveProblemC(fout, movieIDs_to_neighbor_sets_of_5);
	SolveProblemD(fout, userIDs_to_estimated_user_ratings);
	SolveProblemE(fout, userIDs_to_top_5_recommended_movieIDs);
	fout.close();
	std::cout << "Processing Completed" << std::endl;

	return 0;
}