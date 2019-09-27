uint8_t* iconselect(String icon,int id) {
  uint8_t* selected;
  if (icon=="02n" || icon=="03n"){
    selected = i0203n;
  }
  if (icon=="04d" || icon=="04n"){
    selected = i04dn;
  }
  if (icon=="02d" || icon=="03d"){
    selected = i0203d;
  }
  if (icon=="01n"){
    selected = i01n;
  }
  if (icon=="01d"){
    selected = i01d;
  }
  if (icon=="11d" || icon=="11n"){
    selected = i11d11n;
  }
  if (icon=="13d" || icon=="13n"){
    selected = i13dn;
  }
  if (icon=="10d" || icon=="10n"){
    selected = i10d;
  }
  if (icon=="09d" || icon=="09n"){
    selected = showers;
  }
  if (icon=="50d" || icon=="50n"){
    selected = i50dn;
  }
  if (id==202 || id == 212){
    selected = c212232sevthunderstorm;
  }
  if (id == 600){
    selected = c600snowflurry;
  }
  if (id>= 611 && id <= 613){
    selected = sleet;
  }
  return selected;
  
}
