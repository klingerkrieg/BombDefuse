<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class Member extends Model
{
    use HasFactory;

    protected $fillable = ["name", "age", "course", "participate", "participateCount", "team_id"];

    public function team(){
        return $this->belongsTo(Team::class);
    }
    
}
