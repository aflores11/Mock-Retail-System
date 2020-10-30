#include <iostream>
#include "product.h"
#ifndef MOVIE_H
#define MOVIE_H

class Movie : public Product
{
public:
  Movie(const std::string category, const std::string name, double price, int qty, std::string genre, std::string rating);

  ~Movie();

  /**
     * Returns the appropriate keywords that this product should be associated with
     */
  std::set<std::string> keywords() const;

  /**
     * Allows for a more detailed search beyond simple keywords
     */
  bool isMatch(std::vector<std::string> &searchTerms) const;

  /**
     * Returns a string to display the product info for hits of the search
     */
  std::string displayString() const;

  /**
     * Outputs the product info in the database format
     */
  void dump(std::ostream &os) const;

  double getPrice() const;

  std::string getName() const;

  int getQty() const;

  void subtractQty(int num);

private:
  std::string Genre;
  std::string Rating;
  std::set<std::string> moviekeywordset;
};
#endif