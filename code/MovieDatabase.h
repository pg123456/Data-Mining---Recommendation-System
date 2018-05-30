/************************************************************
* Name: Patrick Guo
* ID: 11378369
************************************************************/
#ifndef MOVIEDATABASE_H
#define MOVIEDATABASE_H

#include "StringManipulator.h"
#include "MathHelper.h"
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <set>

struct MoviePairAndCosineSimilarity
{
	int Movie1ID;
	int Movie2ID;
	double Cosine_Similarity;

	MoviePairAndCosineSimilarity(int movie1ID, int movie2ID, double cosine_similarity)
	{
		Movie1ID = movie1ID;
		Movie2ID = movie2ID;
		Cosine_Similarity = cosine_similarity;
	}
};

struct NeighborhoodSetOf5
{
	int Reference_MovieID;
	int Similar_MovieIDs[5];
	double Similarity_Scores[5];

	NeighborhoodSetOf5()
	{
		for (int i = 0; i < 5; i++)
		{
			Similar_MovieIDs[i] = -1;
			Similarity_Scores[i] = 0.0;
		}
	}

	NeighborhoodSetOf5(int reference_movieID)
	{
		Reference_MovieID = reference_movieID;
		for (int i = 0; i < 5; i++)
		{
			Similar_MovieIDs[i] = -1;
			Similarity_Scores[i] = 0.0;
		}
	}

	void AssessSimilarMovie(int movieID_to_assess, double similarity_score_to_assess)
	{
		int least_similar_movie_index = 0;
		double least_similar_cosine_similarity = Similarity_Scores[0];
		for (int i = 1; i < 5; i++)
		{
			/************************************************************
			* In case we compare something we've already compared
			* before, we abort.
			************************************************************/
			if (movieID_to_assess == Similar_MovieIDs[i])
			{
				return;
			}

			if (Similarity_Scores[i] < least_similar_cosine_similarity)
			{
				least_similar_cosine_similarity = Similarity_Scores[i];
				least_similar_movie_index = i;
			}
		}

		if (similarity_score_to_assess > least_similar_cosine_similarity)
		{
			Similarity_Scores[least_similar_movie_index] = similarity_score_to_assess;
			Similar_MovieIDs[least_similar_movie_index] = movieID_to_assess;
		}
	}
};

struct EstimatedUserRatings
{
	int UserID;
	std::vector<int> MovieIDs;
	std::vector<double> Estimated_Ratings;

	EstimatedUserRatings()
	{

	}

	EstimatedUserRatings(int userID)
	{
		UserID = userID;
	}

	void AddEstimatedUserRating(int movieID, double estimated_rating)
	{
		MovieIDs.push_back(movieID);
		Estimated_Ratings.push_back(estimated_rating);
	}
};

struct MovieAverageRating
{
	int MovieID;
	double Average_Rating;

	MovieAverageRating()
	{

	}

	MovieAverageRating(int movieID, double average_rating)
	{
		MovieID = movieID;
		Average_Rating = average_rating;
	}
};

class MovieDatabase
{
private:
	/************************************************************
	* The nested map is userID to rating (for a particular movie)
	* so the format to retrieve a particular user's rating
	* for a particular movie is:
	* double rating = m_movieID_to_ratings[movieID][userID]
	************************************************************/
	std::map<int, std::map<int, double>> m_movieID_to_ratings;
	std::map<int, std::string> m_movieID_to_title;
	std::set<int> m_unique_userIDs;

	bool EstimatedUserRatingAlreadyExists(std::map<int, EstimatedUserRatings> &my_map, int key)
	{
		std::map<int, EstimatedUserRatings>::iterator it;
		it = my_map.find(key);
		if (it != my_map.end())
		{
			return true;
		}

		return false;
	}

public:
	MovieDatabase(std::string movies_file_path, std::string ratings_file_path)
	{
		LoadMoviesData(movies_file_path);
		LoadRatingsData(ratings_file_path);
	}

	void LoadMoviesData(std::string data_file_path)
	{
		m_movieID_to_title.clear();
		std::ifstream fin;
		fin.open(data_file_path);

		/************************************************************
		* Here, each line will be in the format of:
		* movieID, title, genre.
		* Call getline() once since we won't be using the
		* top line
		************************************************************/
		std::string current_line = "";
		getline(fin, current_line);
		std::cout << "Now processing movies into the database..." << std::endl;
		while (fin.good())
		{
			getline(fin, current_line);
			std::vector<std::string> line_pieces =
				StringManipulator::split(current_line, ",");
			if (line_pieces.size() < 3)
			{
				continue;
			}
			int movieID = std::stoi(line_pieces[0]);

			/************************************************************
			* The below section is used for debugging purposes.
			************************************************************/
			/*if (movieID > 100)
			{
				continue;
			}*/

			std::string title = line_pieces[1];
			if (line_pieces.size() > 3)
			{
				for (int i = 2; i < line_pieces.size() - 1; i++)
				{
					title += "," + line_pieces[i];
				}
			}

			m_movieID_to_title[movieID] = title;
		}
		fin.close();
	}

	void LoadRatingsData(std::string data_file_path)
	{
		m_movieID_to_ratings.clear();

		std::ifstream fin;
		fin.open(data_file_path);

		/************************************************************
		* Here, each line will be in the format of:
		* userID, movieID, rating, timestamp.
		* Call getline() once since we won't be using the
		* top line
		************************************************************/
		std::string current_line = "";
		getline(fin, current_line);
		std::cout << "Now processing ratings into the database..." << std::endl;
		while (fin.good())
		{
			getline(fin, current_line);
			std::vector<std::string> line_pieces =
				StringManipulator::split(current_line, ",");
			if (line_pieces.size() < 3)
			{
				continue;
			}
			int userID = std::stoi(line_pieces[0]);
			int movieID = std::stoi(line_pieces[1]);

			/************************************************************
			* The below section is used for debugging purposes.
			************************************************************/
			/*if (movieID > 100)
			{
				continue;
			}*/

			double rating = std::stod(line_pieces[2]);

			m_movieID_to_ratings[movieID][userID] = rating;
			m_unique_userIDs.emplace(userID);
		}
		fin.close();
	}

	std::string GetMovieName(int movieID)
	{
		if (0 == movieID)
		{
			return "";
		}

		return m_movieID_to_title[movieID];
	}

	bool UserHasRatingForMovie(int movieID, int userID)
	{
		return GeneralHelper::MapContainsKey(m_movieID_to_ratings[movieID], userID);
	}

	bool FindMovieAverageRating(std::vector<MovieAverageRating> &movie_average_ratings, MovieAverageRating &target, int &output_index)
	{
		int left_i = 0, right_i = movie_average_ratings.size() - 1;
		output_index = 0;

		while (left_i <= right_i)
		{
			int current_index = (left_i + right_i) / 2;

			if (movie_average_ratings[current_index].Average_Rating < target.Average_Rating)
			{
				left_i = current_index + 1;
				output_index = left_i;
				continue;
			}
			else if (movie_average_ratings[current_index].Average_Rating > target.Average_Rating)
			{
				right_i = current_index - 1;
				output_index = current_index;
				continue;
			}
			else if (target.Average_Rating == movie_average_ratings[current_index].Average_Rating)
			{
				output_index = current_index;
				return true;
			}
		}

		return false;
	}

	void AddMovieAverageRating(std::vector<MovieAverageRating> &movie_average_ratings, MovieAverageRating &target)
	{
		int index = -1;
		FindMovieAverageRating(movie_average_ratings, target, index);

		if (movie_average_ratings.size() == index)
		{
			movie_average_ratings.push_back(target);
		}
		else
		{
			int number_of_elements_to_shift = movie_average_ratings.size() - index;
			movie_average_ratings.resize(movie_average_ratings.size() + 1);
			memcpy(&movie_average_ratings[index + 1], &movie_average_ratings[index], number_of_elements_to_shift * sizeof(MovieAverageRating));
			movie_average_ratings[index] = target;
		}

	}

	void GetStatistics(std::vector<MoviePairAndCosineSimilarity> &movie_pairs_and_cosine_similarities, std::map<int, NeighborhoodSetOf5> &movieIDs_to_neighbor_sets_of_5,
		std::map<int, EstimatedUserRatings> &userIDs_to_estimated_user_ratings, std::map<int, GeneralHelper::ArrayOf5> &userIDs_to_top_5_recommended_movieIDs)
	{
		std::cout << "Now calculating movie pair cosine similarities and neighbor sets..." << std::endl;

		std::map<int, std::map<int, double>>::iterator one_iteration_before_end = m_movieID_to_ratings.end();
		one_iteration_before_end--;

		for (std::map<int, std::map<int, double>>::iterator i = m_movieID_to_ratings.begin(); i != one_iteration_before_end; i++)
		{
			i++;
			for (std::map<int, std::map<int, double>>::iterator j = i--; j != m_movieID_to_ratings.end(); j++)
			{
				int movie1ID = i->first;
				int movie2ID = j->first;
				std::map<int, double> lhs = i->second;
				std::map<int, double> rhs = j->second;

				/***************************************************************
				* Cosine Similarity = (A dot B) / (MagnitudeOfA * MagnitudeOfB)
				***************************************************************/
				double current_pair_cosine_similarity = MathHelper::GetDotProduct(lhs, rhs) / (MathHelper::GetMagnitude(lhs) * MathHelper::GetMagnitude(rhs));
				MoviePairAndCosineSimilarity to_add(movie1ID, movie2ID, current_pair_cosine_similarity);
				movie_pairs_and_cosine_similarities.push_back(to_add);

				/***************************************************************
				* Now do neighbor set calculations
				***************************************************************/
				movieIDs_to_neighbor_sets_of_5[movie1ID].Reference_MovieID = movie1ID;
				movieIDs_to_neighbor_sets_of_5[movie2ID].Reference_MovieID = movie2ID;
				movieIDs_to_neighbor_sets_of_5[movie1ID].AssessSimilarMovie(movie2ID, current_pair_cosine_similarity);
				movieIDs_to_neighbor_sets_of_5[movie2ID].AssessSimilarMovie(movie1ID, current_pair_cosine_similarity);
			}
		}

		/***************************************************************
		* Estimate unknown user ratings
		***************************************************************/
		std::cout << "Now calculating unknown user ratings..." << std::endl;

		for (std::set<int>::iterator i = m_unique_userIDs.begin(); i != m_unique_userIDs.end(); i++)
		{
			int current_userID = *i;

			for (std::map<int, std::map<int, double>>::iterator j = m_movieID_to_ratings.begin(); j != one_iteration_before_end; j++)
			{
				int current_movieID = j->first;
				if (!UserHasRatingForMovie(current_movieID, current_userID))
				{
					if (!EstimatedUserRatingAlreadyExists(userIDs_to_estimated_user_ratings, current_userID))
					{
						EstimatedUserRatings to_add(current_userID);
						userIDs_to_estimated_user_ratings[current_userID] = to_add;
					}
					NeighborhoodSetOf5 current_similar_movies = movieIDs_to_neighbor_sets_of_5[current_movieID];
					double estimated_rating = 0.0, numerator = 0.0, denominator = 0.0;
					for (int k = 0; k < 5; k++)
					{
						int current_movieID_for_comparing = current_similar_movies.Similar_MovieIDs[k];
						double current_similarity_score_for_comparing = current_similar_movies.Similarity_Scores[k];
						if (-1 == current_movieID_for_comparing || !UserHasRatingForMovie(current_movieID_for_comparing, current_userID))
						{
							continue;
						}
						else
						{
							double weight = m_movieID_to_ratings[current_movieID_for_comparing][current_userID];
							numerator += (weight * current_similarity_score_for_comparing);
							denominator += current_similarity_score_for_comparing;
						}
					}
					if (0 != denominator)
					{
						estimated_rating = numerator / denominator;
					}
					userIDs_to_estimated_user_ratings[current_userID].AddEstimatedUserRating(current_movieID, estimated_rating);
				}
			}
		}

		/***************************************************************
		* Find Recommended Movies
		***************************************************************/
		std::cout << "Now finding recommended movies..." << std::endl;

		std::vector<MovieAverageRating> movie_average_ratings;

		for (std::map<int, std::map<int, double>>::iterator i = m_movieID_to_ratings.begin(); i != one_iteration_before_end; i++)
		{
			int current_movieID = i->first;
			double average_rating = 0.0, denominator = i->second.size();
			for (std::map<int, double>::iterator j = i->second.begin(); j != i->second.end(); j++)
			{
				average_rating += j->second;
			}
			if (0 != denominator)
			{
				average_rating /= denominator;
			}
			MovieAverageRating to_add(current_movieID, average_rating);
			AddMovieAverageRating(movie_average_ratings, to_add);
		}

		for (std::set<int>::iterator i = m_unique_userIDs.begin(); i != m_unique_userIDs.end(); i++)
		{
			int current_userID = *i;
			int top_5_movieIDs[5] = { 0, 0, 0, 0, 0 };

			for (int j = movie_average_ratings.size() - 1; j >= 0; j--)
			{
				int current_top_movieID = movie_average_ratings[j].MovieID;
				int current_top_movie_rating = movie_average_ratings[j].Average_Rating;
				if (!UserHasRatingForMovie(current_top_movieID, current_userID))
				{
					for (int k = 0; k < 5; k++)
					{
						if (0 == top_5_movieIDs[k])
						{
							top_5_movieIDs[k] = current_top_movieID;
							break;
						}
					}
				}
				if (0 != top_5_movieIDs[4])
				{
					break;
				}
			}
			GeneralHelper::ArrayOf5 to_add(top_5_movieIDs);
			userIDs_to_top_5_recommended_movieIDs[current_userID] = to_add;
		}
	}
};

#endif