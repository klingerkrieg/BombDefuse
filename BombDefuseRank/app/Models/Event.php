<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Support\Carbon;

class Event extends Model {

    public $timestamps = false;

    protected $fillable = ["name", "time", "type", "team_id"];

    public function team(){
        return $this->belongsTo(Team::class);
    }

    public function getScoreAttribute(){
        if ($this->name == "wire" || $this->name == "code" || $this->name == "defused"){
            $totalTime = Carbon::parse("20:00");
            $time = Carbon::parse($this->time);
			
			if ($this->name == "defused"){
				return $totalTime->diffInMinutes($time)/100 + 100;
			} else {
				return $totalTime->diffInMinutes($time)/100;
			}
        } else 
        if ($this->name == "wrong code" || $this->name == "wrong wire"){
            return -5;
        } else
        if ($this->name == "exploded"){
            return -100;
        }
        else {
            return 0;
        }
    }
    
}
