<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Factories\HasFactory;
use Illuminate\Database\Eloquent\Model;

class TeamQueue extends Model
{
    use HasFactory;

    protected $fillable = [
        'nome',
        'whatsapp',
        'status',
    ];
}
