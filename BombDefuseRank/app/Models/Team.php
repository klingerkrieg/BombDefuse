<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Team extends Model
{
    use HasFactory;

    protected $fillable = ["team_name", "time", "exploded","event_name"];

    public function members(){
        return $this->hasMany(Member::class);
    }

    public function events(){
        return $this->hasMany(Event::class);
    }
    
    public function getScoreAttribute(){
        $score = 0;

        foreach($this->events as $event){
            $score += $event->score;
        }

        return $score;
    }
    
}
